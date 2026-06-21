#include <pch/Precompiled.h>
#include "Storage.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;     // type alias
namespace fs = std::filesystem;  // namespace alias

namespace Storage
{
   void load() {
      std::ifstream s(savefile);
      if(!s.is_open()) {
         TraceLog(LISHA_SAYS, "Game save file not found! Using default values...");
         save();
         return;
      }

      json j;
      try {
         s >> j;
      } catch(const std::exception& e) {
         TraceLog(LOG_ERROR, "Error parsing save file: %s", e.what());
         return;
      }
      s.close();

      game.stage = j["game"]["stage"].get<uint16_t>();
      game.coins = j["game"]["coins"].get<uint32_t>();
      game.bestScore = j["game"]["bestScore"].get<uint32_t>();
      game.currentScore = j["game"]["currentScore"].get<uint32_t>();
      game.numbersCleared = j["game"]["numbersCleared"].get<std::array<bool, 9>>();

      ui.isDarkMode = j["UI"]["isDarkMode"].get<bool>();
   }

   void save() {
      std::string gamesave = std::format(
   R"json({{
      "game": {{
         "stage": {},
         "coins": {},
         "bestScore": {},
         "currentScore": {},
         "numbersCleared": [
            {}, {}, {},
            {}, {}, {},
            {}, {}, {}
         ]
      }},

      "window": {{
         "width": {},
         "height": {}
      }},

      "UI": {{
         "isDarkMode": {}
      }}
   }})json",
         game.stage, game.coins, game.bestScore, game.currentScore,
         game.numbersCleared.at(0), game.numbersCleared.at(1), game.numbersCleared.at(2), 
         game.numbersCleared.at(3), game.numbersCleared.at(4), game.numbersCleared.at(5), 
         game.numbersCleared.at(6), game.numbersCleared.at(7), game.numbersCleared.at(8), 
         
         GetScreenWidth(), GetScreenHeight(),

         ui.isDarkMode
      );

      std::ofstream s(savefile);
      if(!s.is_open()) {
         TraceLog(LOG_ERROR, "Error opening save.json for save!");
         return;
      }

      s << gamesave;
      s.close();
      TraceLog(LISHA_SAYS, "Game info saved at:\n%s", gamesave.c_str());
   }

   std::pair<int, int> getSavedWindowSize() {
      std::ifstream s(savefile);

      if(!s.is_open()) {
         TraceLog(LISHA_SAYS, "Window save file not found! Using default values...");
         return { 800, 650 };
      }

      json j;
      try {
         s >> j;
      } catch(const std::exception& e) {
         TraceLog(LOG_ERROR, "Error parsing window save file: %s", e.what());
         return { 800, 650 };
      }
      s.close();

      int width = j["window"]["width"].get<int>();
      int height = j["window"]["height"].get<int>();

      TraceLog(LISHA_SAYS, "Window loaded at: %d x %d", width, height);
      return { width, height };
   }

   std::string format(uint32_t num) {
      if(num >= 1000) {
         std::ostringstream oss;
         if(num >= 1000000) {
            oss << (num / 1000000) % 1000 << ",";                                    // millions
            oss << std::setfill('0') << std::setw(3) << (num / 1000) % 1000 << ",";  // thousands
         } else
            oss << num / 1000 << ",";  // thousands without leading zeros

         oss << std::setfill('0') << std::setw(3) << num % 1000;  // hundreds
         return oss.str();
      } else
         return std::to_string(num);  // if less than 1000, just return the number as is
   }
} // namespace Storage