#include <pch/Precompiled.h>
#include "Colors.h"

#include "Storage.h"

namespace Palette
{
   void SetLightMode() {
      Storage::isDarkMode = false;
      TraceLog(LISHA_SAYS, "THEME: Light Mode set");

      bright_bg = WHITE;
      shadow_for_bright = LIGHTGRAY;
      text_for_bright = ColorAlpha(DARKERGRAY, 0.98f);
      
      off_bright_bg = RAYWHITE;
      shadow_for_off_bright = ColorAlpha(LIGHTGRAY, 0.98f);
      text_for_off_bright = DARKGRAY;
      
      game_button_bg = LIGHTERGRAY;
      game_button_text = BLUE;
      game_nav_color = BRIGHTSKYBLUE;
      game_info_color = DARKGRAY;
      
      panel_rest = GRAY;
      panel_hover = DARKBLUE;
      panel_active = BLUE;
      
      home_button_1 = WHITE;
      home_button_2 = BLUE;
      
      home_bg_overlay = ColorAlpha(WHITE, 0.16897f);
      darken_game_bg = ColorAlpha(DARKGRAY, 0.706f);
      
      title_color = DARKBLUE;
      title_shadow = Color{ 3, 21, 41, 255 };   // dark navy

      options_bg = LIGHTERGRAY;
      options_title_color = DARKERGRAY;
      options_arrow_color = MEDIUMDARKGRAY;
      options_text_color = DARKGRAY;
      
      gridcell_hover = ColorAlpha(BLIZZARDBLUE, 0.5f);
      gridcell_focus = ColorAlpha(SKYBLUE, 0.5f);
      gridnum_matched = LIGHTGRAY;
      gridnum_not_matched = Color{ 122, 5, 26, 255 };
      gridbox_color = ColorAlpha(DARKGRAY, 0.8f);
      grid_hint = RED;
      
      grid_scroll_thumb = GRAY;
      grid_scroll_track = LIGHTGRAY;
   }

   void SetDarkMode() {
      Storage::isDarkMode = true;
      TraceLog(LISHA_SAYS, "THEME: Dark Mode set");

      bright_bg = Color{ 28, 32, 40, 255 };
      shadow_for_bright = Color{ 70, 75, 90, 255 };
      text_for_bright = Color{ 235, 238, 245, 255 };
      
      off_bright_bg = Color{ 40, 45, 55, 255 };
      shadow_for_off_bright = Color{ 85, 90, 105, 150 };
      text_for_off_bright = Color{ 210, 215, 225, 255 };
      
      game_button_bg = Color{ 55, 60, 75, 255 };
      game_button_text = SKYBLUE;
      game_nav_color = SKYBLUE;
      game_info_color = Color{ 185, 190, 205, 255 };
      
      panel_rest = Color{ 90, 95, 110, 255 };
      panel_hover = SKYBLUE;
      panel_active = CLOUDYBLUE;
      
      home_button_1 = Color{ 50, 55, 70, 255 };
      home_button_2 = CLOUDYBLUE;
      
      home_bg_overlay = ColorAlpha(MEDIUMLIGHTGRAY, 0.16897f);
      darken_game_bg = ColorAlpha(BLACK, 0.706f);
      
      title_color = SKYBLUE;
      title_shadow = DARKBLUE;
      
      options_bg = Color{ 45, 50, 62, 255 };
      options_title_color = Color{ 235, 238, 245, 255 };
      options_arrow_color = Color{ 170, 180, 195, 255 };
      options_text_color = Color{ 210, 215, 225, 255 };
      
      gridcell_hover = Color{ 76, 109, 138, 127 };
      gridcell_focus = Color{ 118, 149, 173, 127 };
      gridnum_matched = Color{ 65, 75, 90, 255 };
      gridnum_not_matched = Color{ 255, 168, 128, 255 };
      gridbox_color = Color{ 120, 140, 180, 90 };
      grid_hint = RED;
      
      grid_scroll_thumb = Color{ 140, 150, 170, 255 };
      grid_scroll_track = Color{ 55, 60, 75, 255 };
   }
} // namespace Palette