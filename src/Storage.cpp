#include <pch/Precompiled.h>
#include "Storage.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;     // type alias
namespace fs = std::filesystem;  // namespace alias

uint32_t Storage::coins = 0;
uint32_t Storage::bestScore = 0;
uint32_t Storage::currentScore = 0;
uint32_t Storage::stage = 1;
std::array<bool, 9> Storage::numbersCleared = {0, 0, 0, 0, 0, 0, 0, 0, 0};

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

   coins = j["coins"].get<uint32_t>();
   bestScore = j["bestScore"].get<uint32_t>();
   currentScore = j["currentScore"].get<uint32_t>();
   stage = j["stage"].get<uint32_t>();
   numbersCleared = j["numbersCleared"].get<std::array<bool, 9>>();
}

bool Storage::hotReload() {
   namespace chr = std::chrono;
   const auto now = chr::steady_clock::now(); // steady_clock bcz it's unaffected by system time changes
   static auto lastCheck = now; 

   if(chr::duration_cast<chr::seconds>(now - lastCheck).count() < 2)
      return false;  // only check every 2 seconds to avoid excessive file system access

   auto currentSaveTime = fs::last_write_time("assets/save.json");

   if(currentSaveTime != lastSaveTime) {
      lastSaveTime = currentSaveTime;
      load();
      lastCheck = now;
      return true;  // file was modified, successfully reloaded
   }

   return false; // file was not modified, no reload needed
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

   save();  // call default save to write updated storage info to file
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