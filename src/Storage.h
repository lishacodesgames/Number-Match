#pragma once

/// @bug save.json is not being saved to repo/assets, it's being saved to bin/assets. Check working directory, it might be set to bin/ instead of repo

namespace Storage {
   using SavedRow = std::array<std::pair<int, std::string>, 9>;
   using SavedGrid = std::vector<SavedRow>;

   struct Game {
      uint16_t stage = 1;
      uint32_t coins = 0;
      uint32_t bestScore = 0;
      uint32_t currentScore = 0;
      std::array<bool, 9> numbersCleared = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /// if 'n' is cleared, [n-1] is true
   };

   struct UI {
      bool isDarkMode = true;
      Font font_retro = GetFontDefault();
      Font font_semibold = GetFontDefault();
      Font font_black = GetFontDefault();
   };

   inline Game game;
   inline UI ui;
   inline const std::filesystem::path savefile = "assets/save.json";

   
   void load(); /// update storage variables from file
   void save(const SavedGrid& grid);  /// update file with storage variables

   std::pair<int, int> getSavedWindowSize(); /// only for App constructor to call
   SavedGrid getSavedGrid(); /// only for Grid constructor to call
}
