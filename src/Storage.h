#include <filesystem>
#include <cstdint>
#include <string>
#include <array>

struct Storage {
   Storage() { lastSaveTime = std::filesystem::last_write_time("assets/save.json"); }

   static uint32_t bestScore;
   static uint32_t coins;
   static uint32_t currentScore;
   static bool isDarkMode;
   static std::array<bool, 9> numbersCleared; /// if 'n' is cleared, [n-1] is true
   static uint32_t stage;

   static std::string formatCoins() { return format(coins); }
   static std::string formatBestScore() { return format(bestScore); }
   static std::string formatCurrentScore() { return format(currentScore); }

   static std::filesystem::file_time_type lastSaveTime;
   static void load(); /// update storage variables from file

   static void save(); /// update file with storage variables

   // only for App to call
   static void saveWindow(int width, int height);
   static std::pair<int, int> getWindowSize();
private:
   static std::string format(uint32_t num); /// perfectly formats bestScore with commas
};