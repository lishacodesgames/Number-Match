#include <pch/Precompiled.h>
#include "Storage.h"

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

std::string Storage::formatCoins() { return format(coins); }
std::string Storage::formatBestScore() { return format(bestScore); }
std::string Storage::formatCurrentScore() { return format(currentScore); }

void Storage::load() {
   std::ifstream file("assets/storage/storage.txt", std::ios::in);
   if(!file.is_open()) return; // if file doesn't exist, just return with default values

   file >> stage;
   for(int i = 0; i < 9; i++)
      file >> numbersCleared[i];
   file >> coins >> bestScore >> currentScore;
   
   file.close();
}

void Storage::save() { 
   std::ofstream file("assets/storage/storage.txt", std::ios::out | std::ios::trunc); // overwrite file with new data
   if(!file.is_open()) return;

   file << stage << "\n";
   for(int i = 0; i < 9; i++)
      file << numbersCleared[i] << (i % 3 == 2 ? "\n" : " "); // new line after every 3 numbers for readability   
   file << coins << "\n" << bestScore << "\n" << currentScore << "\n";

   file.close();
}

void Storage::save(uint32_t stage, std::array<bool, 9> numbersCleared, uint32_t currentScore) {
   Storage::stage = stage;
   Storage::numbersCleared = numbersCleared;
   Storage::currentScore = currentScore;

   save(); // call default save to write updated storage info to file
}