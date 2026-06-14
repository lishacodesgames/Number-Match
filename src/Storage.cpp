#include <pch/Precompiled.h>
#include "Storage.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;     // type alias
namespace fs = std::filesystem;  // namespace alias

uint32_t Storage::bestScore = 0;
uint32_t Storage::coins = 0;
uint32_t Storage::currentScore = 0;
bool Storage::isDarkMode = true;
std::array<bool, 9> Storage::numbersCleared = {0, 0, 0, 0, 0, 0, 0, 0, 0};
uint32_t Storage::stage = 1;

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

   bestScore = j["bestScore"].get<uint32_t>();
   coins = j["coins"].get<uint32_t>();
   currentScore = j["currentScore"].get<uint32_t>();
   isDarkMode = j["isDarkMode"].get<bool>();
   numbersCleared = j["numbersCleared"].get<std::array<bool, 9>>();
   stage = j["stage"].get<uint32_t>();
}

void Storage::save() {
   json j;

   j["bestScore"] = bestScore;
   j["coins"] = coins;
   j["currentScore"] = currentScore;
   j["isDarkMode"] = isDarkMode;
   j["numbersCleared"] = numbersCleared;
   j["stage"] = stage;

   std::ofstream s("assets/save.json");
   if(!s.is_open()) return;

   std::string gamesave = j.dump(3);
   s << gamesave;
   s.close();

   TraceLog(LISHA_SAYS, "Game info saved at:\n%s", gamesave.c_str());
}

void Storage::saveWindow(int width, int height) {
   json j;
   
   j["width"] = width;
   j["height"] = height;

   std::ofstream s("assets/window.json");
   if(!s.is_open()) return;
   s << j.dump(3);
   s.close();

   TraceLog(LISHA_SAYS, "Window size saved at: %d x %d", width, height);
}

std::pair<int, int> Storage::getWindowSize() {
   std::ifstream s("assets/window.json");

   if(!s.is_open()) {
      TraceLog(LISHA_SAYS, "Window save file not found! Using default values...");
      saveWindow(800, 650);
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

   int width = j["width"].get<int>();
   int height = j["height"].get<int>();

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