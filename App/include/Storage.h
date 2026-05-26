#include <filesystem>
#include <cstdint>
#include <string>
#include <array>
#include "Core/Logging.h"

struct Storage {
   Storage();

   static uint32_t stage;
   static std::array<bool, 9> numbersCleared; /// if 'n' is cleared, [n-1] is true
   static uint32_t coins;
   static uint32_t bestScore;
   static uint32_t currentScore;

   inline static std::string formatCoins() { return format(coins); }
   inline static std::string formatBestScore() { return format(bestScore); }
   inline static std::string formatCurrentScore() { return format(currentScore); }

   static bool hotReload(); /// reloads save file if it has been modified since last load/save, returns success
   static void load(); /// update score, numbersCleared and stage from file to storage
   static void save(); /// update file with score, numbersCleared and stage from storage
   static void save(uint32_t stage, std::array<bool, 9> numbersCleared, uint32_t currentScore); /// for saving specific info

   static std::filesystem::file_time_type lastSaveTime;
private:
   static std::string format(uint32_t num); /// perfectly formats bestScore with commas
};