#include <pch/Precompiled.h>
#include "Storage.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

namespace
{
   constexpr int defaultWidth = 800;
   constexpr int defaultHeight = 650;
   const fs::path fallbackSavefile = "assets/start.json";

   std::optional<json> cachedSave;

   /// @param field just for logging errors
   uint64_t getUnsigned(const json& value, uint64_t maximum, const char* field) {
      if(!(value.is_number_unsigned() || value.is_number_integer()))
         throw std::runtime_error(std::string(field) + " must be an unsigned integer");

      const int64_t signedValue = value.get<int64_t>();
      if(signedValue < 0 || static_cast<uint64_t>(signedValue) > maximum)
         throw std::runtime_error(std::string(field) + " is out of range");

      return static_cast<uint64_t>(signedValue);
   }

   void validateSave(const json& document) {
      const json& savedGame = document.at("game");
      const json& savedWindow = document.at("window");
      const json& savedUI = document.at("UI");

      if(getUnsigned(savedGame.at("stage"), UINT16_MAX, "game.stage") == 0)
         throw std::runtime_error("game.stage must be at least 1");
      getUnsigned(savedGame.at("coins"), UINT32_MAX, "game.coins");
      getUnsigned(savedGame.at("bestScore"), UINT32_MAX, "game.bestScore");
      getUnsigned(savedGame.at("currentScore"), UINT32_MAX, "game.currentScore");

      const json& numbersCleared = savedGame.at("numbersCleared");
      if(!numbersCleared.is_array() || numbersCleared.size() != 9)
         throw std::runtime_error("game.numbersCleared must contain exactly 9 booleans");
      for(const json& cleared : numbersCleared)
         if(!cleared.is_boolean())
            throw std::runtime_error("game.numbersCleared must contain only booleans");

      const json& grid = savedGame.at("grid");
      if(!grid.is_array() || grid.size() < 9)
         throw std::runtime_error("game.grid must contain at least 9 rows");
      for(const json& row : grid) {
         if(!row.is_array() || row.size() != 9)
            throw std::runtime_error("every game.grid row must contain exactly 9 cells");
         for(const json& cell : row) {
            if(!cell.is_array() || cell.size() != 2)
               throw std::runtime_error("every grid cell must be [value, state]");
            getUnsigned(cell.at(0), 9, "grid cell value");
            if(!cell.at(1).is_string())
               throw std::runtime_error("grid cell state must be a string");
            const std::string state = cell.at(1).get<std::string>();
            if(state != "Rest" && state != "Matched")
               throw std::runtime_error("grid cell state must be Rest or Matched");
         }
      }

      if(getUnsigned(savedWindow.at("width"), INT_MAX, "window.width") == 0 ||
         getUnsigned(savedWindow.at("height"), INT_MAX, "window.height") == 0)
         throw std::runtime_error("saved window dimensions must be positive");
      if(!savedUI.at("isDarkMode").is_boolean())
         throw std::runtime_error("UI.isDarkMode must be a boolean");
   }

   json readAndValidate(const fs::path& path) {
      std::ifstream stream(path);
      if(!stream.is_open())
         throw fs::filesystem_error(
            "Could not open save file", path, std::make_error_code(std::errc::no_such_file_or_directory));

      json document;
      stream >> document;
      validateSave(document);
      return document;
   }

   fs::path getCorruptBackupPath() {
      fs::path backup = Storage::savefile;
      backup += ".corrupt";
      for(unsigned int suffix = 1; fs::exists(backup); suffix++) {
         backup = Storage::savefile;
         backup += ".corrupt." + std::to_string(suffix);
      }
      return backup;
   }

   bool installDefaultSave() {
      std::error_code error;
      fs::create_directories(Storage::savefile.parent_path(), error);
      error.clear();
      fs::copy_file(fallbackSavefile, Storage::savefile, fs::copy_options::overwrite_existing, error);

      if(error) {
         TraceLog(LOG_ERROR, "Could not install default save: %s", error.message().c_str());
         return false;
      }

      return true;
   }

   bool loadSaveDocument() {
      if(!fs::exists(Storage::savefile)) {
         TraceLog(LISHA_SAYS, "Game save file not found! Using default values...");
         if(!installDefaultSave())
            return false;
      }

      try {
         cachedSave = readAndValidate(Storage::savefile);
         return true;
      } catch(const std::exception& e) {
         TraceLog(LOG_ERROR, "Invalid save file: %s", e.what());
      }

      std::error_code error;
      const fs::path backup = getCorruptBackupPath();
      fs::rename(Storage::savefile, backup, error);
      if(error) {
         TraceLog(LOG_ERROR, "Could not preserve corrupt save: %s", error.message().c_str());
         return false;
      }
      TraceLog(LISHA_SAYS, "Corrupt save backed up to: %s", backup.string().c_str());

      if(!installDefaultSave())
         return false;

      try {
         cachedSave = readAndValidate(Storage::savefile);
         return true;
      } catch(const std::exception& e) {
         TraceLog(LOG_ERROR, "Default save is invalid: %s", e.what());
         return false;
      }
   }

   const json* getSaveDocument() {
      if(!cachedSave && !loadSaveDocument())
         return nullptr;
      return &*cachedSave;
   }

   Storage::SavedGrid makeEmptyGrid() {
      Storage::SavedRow row;
      row.fill({ 0, "Rest" });
      return Storage::SavedGrid(9, row);
   }
}

namespace Storage
{
   void load() {
      const json* document = getSaveDocument();
      if(!document)
         return;

      const json& savedGame = document->at("game");
      game.stage = savedGame.at("stage").get<uint16_t>();
      game.coins = savedGame.at("coins").get<uint32_t>();
      game.bestScore = savedGame.at("bestScore").get<uint32_t>();
      game.currentScore = savedGame.at("currentScore").get<uint32_t>();
      game.numbersCleared = savedGame.at("numbersCleared").get<std::array<bool, 9>>();
      ui.isDarkMode = document->at("UI").at("isDarkMode").get<bool>();
   }

   void save(const SavedGrid& grid) {
      json j = {
         { "game", {
            { "stage", game.stage },
            { "coins", game.coins },
            { "bestScore", game.bestScore },
            { "currentScore", game.currentScore },
            { "numbersCleared", game.numbersCleared },
            { "grid", grid }
         } },
         { "window", {
            { "width", GetScreenWidth() },
            { "height", GetScreenHeight() }
         } },
         { "UI", { { "isDarkMode", ui.isDarkMode } } }
      };

      try {
         validateSave(j);
      } catch(const std::exception& e) {
         TraceLog(LOG_ERROR, "Refusing to write invalid save: %s", e.what());
         return;
      }

      fs::path temporary = savefile;
      temporary += ".tmp";
      std::ofstream stream(temporary, std::ios::trunc);
      if(!stream.is_open()) {
         TraceLog(LOG_ERROR, "Could not open temporary save file");
         return;
      }

      stream << j.dump(3) << '\n';
      stream.flush();
      if(!stream.good()) {
         TraceLog(LOG_ERROR, "Failed while writing temporary save file");
         stream.close();
         std::error_code ignored;
         fs::remove(temporary, ignored);
         return;
      }
      stream.close();

      std::error_code error;
      fs::rename(temporary, savefile, error);
      if(error) {
         // std::filesystem has no portable replace operation; this fallback is for platforms
         // where rename cannot replace an existing destination.
         std::error_code removeError;
         fs::remove(savefile, removeError);
         error.clear();
         fs::rename(temporary, savefile, error);
      }
      if(error) {
         TraceLog(LOG_ERROR, "Could not replace save file: %s", error.message().c_str());
         return;
      }

      cachedSave = std::move(j);
      TraceLog(LISHA_SAYS, "Game saved successfully");
   }

   std::pair<int, int> getSavedWindowSize() {
      const json* document = getSaveDocument();
      if(!document)
         return { defaultWidth, defaultHeight };

      const int width = document->at("window").at("width").get<int>();
      const int height = document->at("window").at("height").get<int>();
      TraceLog(LISHA_SAYS, "Window loaded at: %d x %d", width, height);
      return { width, height };
   }

   SavedGrid getSavedGrid() {
      const json* document = getSaveDocument();
      if(!document)
         return makeEmptyGrid();
      return document->at("game").at("grid").get<SavedGrid>();
   }

   std::string format(uint32_t num) {
      std::string result = std::to_string(num);
      for(std::ptrdiff_t position = static_cast<std::ptrdiff_t>(result.size()) - 3;
            position > 0; position -= 3)
         result.insert(static_cast<size_t>(position), ",");
      return result;
   }
} // namespace Storage
