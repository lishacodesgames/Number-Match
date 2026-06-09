#pragma once
#include <raylib.h>

// colors
constexpr Color LIGHTERGRAY = {230, 230, 230, 255}; /// A more aesthetically pleasing lightgray
constexpr Color MEDIUMLIGHTGRAY = {180, 180, 180, 255}; /// Between raylib's LIGHTGRAY and GRAY
constexpr Color MEDIUMDARKGRAY = {110, 110, 110, 255}; /// Between raylib's GRAY and DARKGRAY
constexpr Color DARKERGRAY = {50, 50, 50, 255}; /// A more aesthetically pleasing darkgray
constexpr Color BLIZZARDBLUE = {172, 229, 238, 255}; /// A pleasant pale lightblue
constexpr Color BRIGHTSKYBLUE = {42, 187, 235, 255}; /// Bright lightblue

// palette
#ifdef DARKMODE

#else
   #define BRIGHT_BG WHITE
   #define SHADOW_FOR_BRIGHT LIGHTGRAY
   #define TEXT_FOR_BRIGHT Color{50, 50, 50, 249} // ColorAlpha(DARKERGRAY, 0.98f)
   
   #define OFF_BRIGHT_BG RAYWHITE
   #define TEXT_FOR_OFF_BRIGHT DARKGRAY

   #define GAME_BUTTON_BG LIGHTERGRAY
   #define GAME_BUTTON_TEXT BLUE
   #define GAME_NAV_COLOR BRIGHTSKYBLUE
   #define GAME_INFO_COLOR DARKGRAY

   #define HOME_BUTTON_1 WHITE
   #define HOME_BUTTON_2 BLUE

   #define HOME_BG_OVERLAY Color{255, 255, 255, 43} // ColorAlpha(WHITE, 0.16897f)
   #define DARKEN_GAME_BG Color{80, 80, 80, 180}

   #define TITLE_COLOR DARKBLUE
   #define TITLE_SHADOW Color{3, 21, 41, 255}

   #define OPTIONS_BG LIGHTERGRAY
#endif