#pragma once
#include <Button.h>

struct MenuPanel {
   MenuPanel();
   void Draw();
   
   static constexpr float HEIGHT = 50.0f;

   Button homeButton;
   Button dailyButton;
   Button meButton;

   Button* findActiveButton();
};