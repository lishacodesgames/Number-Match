#pragma once
#include "Button.h"
#include "Layer.h"

struct MenuPanel {
   MenuPanel();
   void Update();
   void Draw();
   
   static constexpr float HEIGHT = 50.0f;
   static constexpr float BUTTON_SPACING = 192.0f;

   Button homeButton;
   Button dailyButton;
   Button meButton;

   Button* findHoveredButton();
   Button* findActiveButton();
   Button* findFocusedButton();

   void resetAllFocus();
};