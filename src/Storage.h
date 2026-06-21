#include <filesystem>
#include <cstdint>
#include <string>
#include <array>

namespace Storage {
   struct Game {
      uint16_t stage = 1;
      uint32_t coins = 0;
      uint32_t bestScore = 0;
      uint32_t currentScore = 0;
      std::array<bool, 9> numbersCleared = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /// if 'n' is cleared, [n-1] is true
   };

   struct UI {
      // more to come like scroll direction preferences, etc
      bool isDarkMode = true;
   };

   inline Game game;
   inline UI ui;
   inline std::filesystem::path savefile = "assets/save.json";
   inline std::filesystem::file_time_type lastSaveTime = std::filesystem::last_write_time(savefile);

   void load();  /// update storage variables from file
   void save();  /// update file with storage variables

   std::pair<int, int> getSavedWindowSize(); /// only for App constructor to call

   /// @todo extract into Engine/Utils/Strings.h
   std::string format(uint32_t num);  /// perfectly formats bestScore with commas
}  // namespace Storage