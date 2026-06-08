#include <filesystem>
#include <cstdint>
#include <string>
#include <array>

struct Storage {
   Storage() { lastSaveTime = std::filesystem::last_write_time("assets/save.json"); }

   static uint32_t stage;
   static std::array<bool, 9> numbersCleared; /// if 'n' is cleared, [n-1] is true
   static uint32_t coins;
   static uint32_t bestScore;
   static uint32_t currentScore;

   static std::string formatCoins() { return format(coins); }
   static std::string formatBestScore() { return format(bestScore); }
   static std::string formatCurrentScore() { return format(currentScore); }

   static std::filesystem::file_time_type lastSaveTime;
   static void load(); /// update score, numbersCleared and stage from file to storage
   static bool hotReload(); /// reloads save file if it has been modified since last load/save, returns success

   static void save(); /// update file with score, numbersCleared and stage from storage
   static void save(uint32_t stage, std::array<bool, 9> numbersCleared, uint32_t currentScore); /// for saving specific info

   // only for App to call
   static void saveWindow(int width, int height);
   static std::pair<int, int> getWindowSize();
private:
   static std::string format(uint32_t num); /// perfectly formats bestScore with commas
};