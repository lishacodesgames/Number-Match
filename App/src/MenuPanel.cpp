#include <Precompiled.h>
#include "MenuPanel.h"

#include <raymath.h>
#include <raylib.h>
#include <Button.h>

MenuPanel::MenuPanel() {
   constexpr float BUTTON_SPACING = 192.0f;
   Vector2 panelButtonsOrigin = { 
      (float)(GetScreenWidth()) / 3 - 150, (float)(GetScreenHeight()) - MenuPanel::HEIGHT + 15
   };

   homeButton = Button( panelButtonsOrigin, {0, 0}, "Main", BLANK, BLUE);
   dailyButton = Button( panelButtonsOrigin + Vector2{BUTTON_SPACING, 0}, {0, 0}, "Daily Challenges", BLANK, GRAY);
   meButton = Button( panelButtonsOrigin + Vector2{BUTTON_SPACING*2.7f, 0}, {0, 0}, "Me", BLANK, GRAY);
}

void MenuPanel::Draw() {
   DrawLine(0, GetScreenHeight() - MenuPanel::HEIGHT, GetScreenWidth(), GetScreenHeight() - MenuPanel::HEIGHT, DARKGRAY);
   DrawRectangleV(
      {0.0f, (float)(GetScreenHeight() - MenuPanel::HEIGHT)}, 
      {(float)GetScreenWidth(), MenuPanel::HEIGHT}, WHITE
   );

   homeButton.Draw();
   dailyButton.Draw();
   meButton.Draw();
}

Button* MenuPanel::findActiveButton() {
   if(homeButton.isActive)
      return &homeButton;
   else if(dailyButton.isActive)
      return &dailyButton;
   else if(meButton.isActive)
      return &meButton;
   else  
      return nullptr;
}