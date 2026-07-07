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

   json read(const fs::path& path) {
      std::ifstream stream(path);
      if(!stream.is_open())
         throw fs::filesystem_error(
            "Could not open save file", path, std::make_error_code(std::errc::no_such_file_or_directory));

      Core::ConsoleLog(LISHA_SAYS, std::format("Reading {}...", fs::absolute(path).string()));
      json document;
      stream >> document;
      stream.close();
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
         Core::ConsoleLog(LOG_ERROR, std::format("Could not install default save: {}", error.message()));
         return false;
      }

      return true;
   }

   bool loadSaveToCache() {
      if(!fs::exists(Storage::savefile)) {
         Core::ConsoleLog(LISHA_SAYS, "Game save file not found! Using default values...");
         if(!installDefaultSave())
            return false;
      }

      try {
         cachedSave = read(Storage::savefile);
         return true;
      } catch(const std::exception& e) {
         Core::ConsoleLog(LOG_ERROR, std::format("Invalid save file: {}", e.what()));
      }

      std::error_code error;
      const fs::path backup = getCorruptBackupPath();
      fs::rename(Storage::savefile, backup, error);
      if(error) {
         Core::ConsoleLog(LOG_ERROR, std::format("Could not preserve corrupt save: {}", error.message()));
         return false;
      }
      Core::ConsoleLog(LISHA_SAYS, std::format("Corrupt save backed up to: {}", backup.string()));

      if(!installDefaultSave())
         return false;

      try {
         cachedSave = read(Storage::savefile);
         return true;
      } catch(const std::exception& e) {
         Core::ConsoleLog(LOG_ERROR, std::format("Default save is invalid: {}", e.what()));
         return false;
      }
   }

   const json* getSaveDocument() {
      if(!cachedSave && !loadSaveToCache())
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
      if(!document) {
         Core::ConsoleLog(LOG_ERROR, "Could not load save file!");
         return;
      }

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

      fs::path temporary = savefile;
      temporary += ".tmp";
      std::ofstream stream(temporary, std::ios::trunc);
      if(!stream.is_open()) {
         Core::ConsoleLog(LOG_ERROR, "Could not open temporary save file");
         return;
      }

      stream << j.dump(3) << '\n';
      stream.flush();
      if(!stream.good()) {
         Core::ConsoleLog(LOG_ERROR, "Failed while writing temporary save file");
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
         Core::ConsoleLog(LOG_ERROR, std::format("Could not replace save file: {}", error.message()));
         return;
      }

      cachedSave = std::move(j);
      Core::ConsoleLog(LISHA_SAYS, "Game saved successfully");
      Core::ConsoleLog(LISHA_TRACE, std::format("Save location: {}", fs::absolute(savefile).string()));
   }

   std::pair<int, int> getSavedWindowSize() {
      const json* document = getSaveDocument();
      if(!document)
         return { defaultWidth, defaultHeight };

      const int width = document->at("window").at("width").get<int>();
      const int height = document->at("window").at("height").get<int>();
      Core::ConsoleLog(LISHA_SAYS, std::format("Window loaded at: {} x {}", width, height));
      return { width, height };
   }

   SavedGrid getSavedGrid() {
      const json* document = getSaveDocument();
      if(!document)
         return makeEmptyGrid();
      return document->at("game").at("grid").get<SavedGrid>();
   }
} // namespace Storage
