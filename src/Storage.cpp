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
   std::ifstream save("assets/save.json");
   if(!save.is_open()) {
      TraceLog(LOG_ERROR, "Error opening save file for parse!");
      return;
   }
   json j;
   try {
      save >> j;
   } catch(const std::exception& e) {
      TraceLog(LOG_ERROR, "Error parsing save file: %s", e.what());
      return;
   }
   save.close();

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

   std::ofstream save("assets/save.json");
   if(!save.is_open()) return;
   save << j.dump(3);
   save.close();

   TraceLog(LISHA_SAYS, "\nGame info saved:\n%s", j.dump(3).c_str());
}

void Storage::save(uint32_t stage, std::array<bool, 9> numbersCleared, uint32_t currentScore) {
   Storage::stage = stage;
   Storage::numbersCleared = numbersCleared;
   Storage::currentScore = currentScore;

   save();  // call default save to write updated storage info to file
}

void Storage::saveWindow(int width, int height) {
   json j;
   
   j["width"] = width;
   j["height"] = height;

   std::ofstream window("assets/window.json");
   if(!window.is_open()) return;
   window << j.dump(3);
   window.close();

   TraceLog(LISHA_SAYS, "Window size saved at: %d x %d", width, height);
}

std::pair<int, int> Storage::getWindowSize() {
   std::ifstream window("assets/window.json");

   if(!window.is_open()) {
      TraceLog(LISHA_SAYS, "Error opening window save file for parse! Using default values...");
      Storage::saveWindow(800, 650);
      return { 800, 650 };
   }

   json j;
   try {
      window >> j;
   } catch(const std::exception& e) {
      TraceLog(LOG_ERROR, "Error parsing window save file: %s", e.what());
      return { 800, 650 };
   }
   window.close();

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