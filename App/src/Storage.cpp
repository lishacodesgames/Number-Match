#include <pch/Precompiled.h>
#include "Storage.h"

#include "json.hpp"
using json = nlohmann::json;

uint32_t Storage::coins = 0;
uint32_t Storage::bestScore = 0;
uint32_t Storage::currentScore = 0;
uint32_t Storage::stage = 1;
std::array<bool, 9> Storage::numbersCleared = {0, 0, 0, 0, 0, 0, 0, 0, 0};

std::string Storage::format(uint32_t num) {
   if(num >= 1000) {
      std::ostringstream oss;
      if(num >= 1000000) {
         oss << (num / 1000000) % 1000 << ","; // millions
         oss << std::setfill('0') << std::setw(3) << (num / 1000) % 1000 << ","; // thousands
      } else
         oss << num / 1000 << ","; // thousands without leading zeros

      oss << std::setfill('0') << std::setw(3) << num % 1000; // hundreds
      return oss.str();
   } else
      return std::to_string(num); // if less than 1000, just return the number as is
}

void Storage::load() {
   std::ifstream save("assets/save.json");
   if(!save.is_open()) return;
   json j;
   try {
      save >> j;
   } catch(const std::exception& e) {
      TraceLog(LOG_ERROR, "Error parsing save file: %s", e.what());
      return;
   }
   save.close();
   
   coins = j["coins"].get<uint32_t>();
   bestScore = j["bestScore"].get<uint32_t>();
   currentScore = j["currentScore"].get<uint32_t>();
   stage = j["stage"].get<uint32_t>();
   numbersCleared = j["numbersCleared"].get<std::array<bool, 9>>();
}

void Storage::save() { 
   json j;

   j["coins"] = coins;
   j["bestScore"] = bestScore;
   j["currentScore"] = currentScore;
   j["stage"] = stage;
   j["numbersCleared"] = numbersCleared;
   
   std::ofstream save("assets/save.json");
   if(!save.is_open()) return;
   save << j.dump(3);
   save.close();
}

void Storage::save(uint32_t stage, std::array<bool, 9> numbersCleared, uint32_t currentScore) {
   Storage::stage = stage;
   Storage::numbersCleared = numbersCleared;
   Storage::currentScore = currentScore;

   save(); // call default save to write updated storage info to file
}