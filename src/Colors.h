#pragma once
#include <raylib.h>

// colors
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
   extern Color bright_bg;
   extern Color shadow_for_bright;
   extern Color text_for_bright;

   extern Color off_bright_bg;
   extern Color shadow_for_off_bright;
   extern Color text_for_off_bright;

   extern Color game_button_bg;
   extern Color game_button_text;
   extern Color game_nav_color;
   extern Color game_info_color;

   extern Color panel_rest;
   extern Color panel_hover;
   extern Color panel_active;

   extern Color home_button_1;
   extern Color home_button_2;

   extern Color home_bg_overlay;
   extern Color darken_game_bg;

   extern Color title_color;
   extern Color title_shadow;

   extern Color options_bg;
   extern Color options_title_color;
   extern Color options_arrow_color;
   extern Color options_text_color;

   extern Color gridcell_hover;
   extern Color gridcell_focus;
   extern Color gridnum_matched;
   extern Color gridnum_not_matched;
   extern Color gridbox_color;

   extern Color grid_scroll_thumb;
   extern Color grid_scroll_track;
}  // namespace Palette