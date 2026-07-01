#pragma once
#include <raylib.h>

constexpr Color LIGHTERGRAY = { 230, 230, 230, 255 };      /// A more aesthetically pleasing lightgray
constexpr Color MEDIUMLIGHTGRAY = { 180, 180, 180, 255 };  /// Between raylib's LIGHTGRAY and GRAY
constexpr Color MEDIUMDARKGRAY = { 110, 110, 110, 255 };   /// Between raylib's GRAY and DARKGRAY
constexpr Color DARKERGRAY = { 50, 50, 50, 255 };          /// A more aesthetically pleasing darkgray

constexpr Color CLOUDYBLUE = { 78, 143, 208, 255 };     /// A soft cobalt blue
constexpr Color BLIZZARDBLUE = { 172, 229, 238, 255 };  /// A pleasant pale lightblue
constexpr Color BRIGHTSKYBLUE = { 42, 187, 235, 255 };  /// Bright lightblue

namespace Palette {
   void SetLightMode();
   void SetDarkMode();

   // default is purple bcz it's not used anywhere else
   inline Color bright_bg = PURPLE;
   inline Color shadow_for_bright = PURPLE;
   inline Color text_for_bright = PURPLE;

   inline Color off_bright_bg = PURPLE;
   inline Color shadow_for_off_bright = PURPLE;
   inline Color text_for_off_bright = PURPLE;

   inline Color game_button_bg = PURPLE;
   inline Color game_button_text = PURPLE;
   inline Color game_nav_color = PURPLE;
   inline Color game_info_color = PURPLE;

   inline Color panel_rest = PURPLE;
   inline Color panel_hover = PURPLE;
   inline Color panel_active = PURPLE;

   inline Color home_button_1 = PURPLE;
   inline Color home_button_2 = PURPLE;

   inline Color home_bg_overlay = PURPLE;
   inline Color darken_game_bg = PURPLE;

   inline Color title_color = PURPLE;
   inline Color title_shadow = PURPLE;

   inline Color options_bg = PURPLE;
   inline Color options_title_color = PURPLE;
   inline Color options_arrow_color = PURPLE;
   inline Color options_text_color = PURPLE;

   inline Color gridcell_hover = PURPLE;
   inline Color gridcell_focus = PURPLE;
   inline Color gridnum_matched = PURPLE;
   inline Color gridnum_not_matched = PURPLE;
   inline Color gridbox_color = PURPLE;
   inline Color grid_hint = PURPLE;

   inline Color grid_scroll_thumb = PURPLE;
   inline Color grid_scroll_track = PURPLE;
}  // namespace Palette