#include <pch/Precompiled.h>
#include "Storage.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;     // type alias
namespace fs = std::filesystem;  // namespace alias

uint32_t Storage::bestScore = 0;
uint32_t Storage::coins = 0;
uint32_t Storage::currentScore = 0;
bool Storage::isDarkMode = true;
std::array<bool, 9> Storage::numbersCleared = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uint16_t Storage::stage = 1;

fs::file_time_type Storage::lastSaveTime = fs::file_time_type::min();

void Storage::load() {
   std::ifstream s("assets/save.json");
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

   stage = j["game"]["stage"].get<uint16_t>();
   coins = j["game"]["coins"].get<uint32_t>();
   bestScore = j["game"]["bestScore"].get<uint32_t>();
   currentScore = j["game"]["currentScore"].get<uint32_t>();
   numbersCleared = j["game"]["numbersCleared"].get<std::array<bool, 9>>();

   isDarkMode = j["UI"]["isDarkMode"].get<bool>();
}

void Storage::save() {
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
      stage, coins, bestScore, currentScore,
      numbersCleared.at(0), numbersCleared.at(1), numbersCleared.at(2), 
      numbersCleared.at(3), numbersCleared.at(4), numbersCleared.at(5), 
      numbersCleared.at(6), numbersCleared.at(7), numbersCleared.at(8), 
      
      GetScreenWidth(), GetScreenHeight(),

      isDarkMode
   );

   std::ofstream s("assets/save.json");
   if(!s.is_open()) {
      TraceLog(LOG_ERROR, "Error opening save.json for save!");
      return;
   }

   s << gamesave;
   s.close();
   TraceLog(LISHA_SAYS, "Game info saved at:\n%s", gamesave.c_str());
}

std::pair<int, int> Storage::getSavedWindowSize() {
   std::ifstream s("assets/save.json");

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

std::string Storage::format(uint32_t num) {
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