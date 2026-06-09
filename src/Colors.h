#pragma once
#include <raylib.h>

// colors
constexpr Color LIGHTERGRAY = { 230, 230, 230, 255 };       /// A more aesthetically pleasing lightgray
constexpr Color MEDIUMLIGHTGRAY = { 180, 180, 180, 255 };   /// Between raylib's LIGHTGRAY and GRAY
constexpr Color MEDIUMDARKGRAY = { 110, 110, 110, 255 };    /// Between raylib's GRAY and DARKGRAY
constexpr Color DARKERGRAY = { 50, 50, 50, 255 };           /// A more aesthetically pleasing darkgray
constexpr Color BLIZZARDBLUE = { 172, 229, 238, 255 };      /// A pleasant pale lightblue
constexpr Color BRIGHTSKYBLUE = { 42, 187, 235, 255 };      /// Bright lightblue

// palette
#define DARKMODE
#ifdef DARKMODE
   #define BRIGHT_BG             Color{ 28, 32, 40, 255 }       /// Deep blue-gray
   #define SHADOW_FOR_BRIGHT     Color{ 70, 75, 90, 255 }       /// Medium blue-gray
   #define TEXT_FOR_BRIGHT       Color{ 235, 238, 245, 255 }    /// Soft off-white

   #define OFF_BRIGHT_BG         Color{ 40, 45, 55, 255 }       /// Dark slate blue-gray
   #define SHADOW_FOR_OFF_BRIGHT Color{ 85, 90, 105, 150 }      /// Muted steel blue-gray
   #define TEXT_FOR_OFF_BRIGHT   Color{ 210, 215, 225, 255 }    /// Light cool gray

   #define GAME_BUTTON_BG        Color{ 55, 60, 75, 255 }       /// Charcoal blue-gray
   #define GAME_BUTTON_TEXT      SKYBLUE                        /// Bright sky blue
   #define GAME_NAV_COLOR        BRIGHTSKYBLUE                 /// Vibrant cyan-blue
   #define GAME_INFO_COLOR       Color{ 185, 190, 205, 255 }   /// Pale blue-gray

   #define PANEL_REST            Color{ 90, 95, 110, 255 }     /// Cool medium gray
   #define PANEL_HOVER           SKYBLUE                        /// Bright sky blue
   #define PANEL_ACTIVE          BLUE                           /// Strong royal blue

   #define HOME_BUTTON_1         Color{ 50, 55, 70, 255 }      /// Dark navy-gray
   #define HOME_BUTTON_2         BLUE                           /// Strong royal blue

   #define HOME_BG_OVERLAY       Color{ 200, 200, 200, 43 }          /// Translucent black
   #define DARKEN_GAME_BG        Color{ 0, 0, 0, 180 }         /// Semi-opaque black

   #define TITLE_COLOR           SKYBLUE                        /// Bright sky blue
   #define TITLE_SHADOW          DARKBLUE                       /// Deep navy blue

   #define OPTIONS_BG            Color{ 45, 50, 62, 255 }      /// Dark steel blue-gray
   #define OPTIONS_TITLE_COLOR   Color{ 235, 238, 245, 255 }   /// Soft off-white
   #define OPTIONS_ARROW_COLOR   Color{ 170, 180, 195, 255 }   /// Light silver-blue
   #define OPTIONS_TEXT_COLOR    Color{ 210, 215, 225, 255 }   /// Light cool gray

   #define GRIDCELL_HOVER        Color{ 76, 109, 138, 127 }   /// Pale icy blue
   #define GRIDCELL_FOCUS        Color{ 118, 149, 173, 127 }   /// Bright azure blue
   #define GRIDCELL_MATCHED      Color{ 65, 75, 90, 255 }      /// Dark slate blue-gray
   #define GRIDCELL_NOT_MATCHED  Color{ 255, 87, 87, 255 }   /// Soft coral red
   #define GRIDBOX_COLOR         Color{ 120, 140, 180, 90 }    /// Dusty steel blue

   #define GRID_SCROLL_THUMB     Color{ 140, 150, 170, 255 }   /// Muted silver-blue
   #define GRID_SCROLL_TRACK     Color{ 55, 60, 75, 255 }      /// Charcoal blue-gray
#else
   #define BRIGHT_BG             WHITE
   #define SHADOW_FOR_BRIGHT     LIGHTGRAY
   #define TEXT_FOR_BRIGHT       Color{ 50, 50, 50, 249 }      // ColorAlpha(DARKERGRAY, 0.98f)
   
   #define OFF_BRIGHT_BG         RAYWHITE
   #define SHADOW_FOR_OFF_BRIGHT Color{ 200, 200, 200, 166 }   // ColorAlpha(LIGHTGRAY, 0.65f)
   #define TEXT_FOR_OFF_BRIGHT   DARKGRAY

   #define GAME_BUTTON_BG        LIGHTERGRAY
   #define GAME_BUTTON_TEXT      BLUE
   #define GAME_NAV_COLOR        BRIGHTSKYBLUE
   #define GAME_INFO_COLOR       DARKGRAY

   #define PANEL_REST            GRAY
   #define PANEL_HOVER           DARKBLUE
   #define PANEL_ACTIVE          BLUE

   #define HOME_BUTTON_1         WHITE
   #define HOME_BUTTON_2         BLUE

   #define HOME_BG_OVERLAY       Color{ 255, 255, 255, 43 }    // ColorAlpha(WHITE, 0.16897f)
   #define DARKEN_GAME_BG        Color{ 80, 80, 80, 180 }      // ColorAlpha(DARKGRAY, 0.706f)

   #define TITLE_COLOR           DARKBLUE
   #define TITLE_SHADOW          Color{ 3, 21, 41, 255 }

   #define OPTIONS_BG            LIGHTERGRAY
   #define OPTIONS_TITLE_COLOR   DARKERGRAY
   #define OPTIONS_ARROW_COLOR   MEDIUMDARKGRAY
   #define OPTIONS_TEXT_COLOR    DARKGRAY

   #define GRIDCELL_HOVER        Color{ 172, 229, 238, 127 }   // ColorAlpha(BLIZZARDBLUE, 0.5f)
   #define GRIDCELL_FOCUS        Color{ 102, 191, 255, 127 }   // ColorAlpha(SKYBLUE, 0.5f)
   #define GRIDCELL_MATCHED      LIGHTGRAY
   #define GRIDCELL_NOT_MATCHED  BLACK
   #define GRIDBOX_COLOR         Color{ 80, 80, 80, 204 } // ColorAlpha(DARKGRAY, 0.8f)
   
   #define GRID_SCROLL_THUMB     GRAY
   #define GRID_SCROLL_TRACK     LIGHTGRAY
#endif