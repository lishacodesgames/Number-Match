#include <Precompiled.h>
#include "MenuPanel.h"

#include <raymath.h>
#include <raylib.h>
#include "Button.h"

static constexpr float BUTTON_SPACING = 192.0f;

static Vector2 buttonsOrigin() { // must be compuled after window exists, hence the function
   return { 
      static_cast<float>(GetScreenWidth() / 3 - 150), 
      static_cast<float>(GetScreenHeight() - MenuPanel::HEIGHT + 15)
   };
}

MenuPanel::MenuPanel() :
      homeButton(buttonsOrigin(), {0, 0}, "Main", BLANK, BLUE),
      dailyButton(buttonsOrigin() + Vector2{BUTTON_SPACING, 0}, {0, 0}, "Daily Challenges", BLANK, GRAY),
      meButton(buttonsOrigin() + Vector2{BUTTON_SPACING*2.7f, 0}, {0, 0}, "Me", BLANK, GRAY)
{}

void MenuPanel::Update(Button* focusedButton) {
   homeButton.Update();
   dailyButton.Update();
   meButton.Update();

   homeButton.setFocus(false, BLANK, GRAY);
   dailyButton.setFocus(false, BLANK, GRAY);
   meButton.setFocus(false, BLANK, GRAY);
   
   focusedButton->setFocus(true, BLANK, BLUE);

   Button* hoveredButton = findHoveredButton();
   if(hoveredButton) {
      hoveredButton->textColor = DARKBLUE;
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   } else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void MenuPanel::Draw() {
   DrawLine(0, GetScreenHeight() - MenuPanel::HEIGHT, GetScreenWidth(), GetScreenHeight() - MenuPanel::HEIGHT, {180, 180, 180, 255});
   DrawRectangleV(
      {0.0f, static_cast<float>(GetScreenHeight() - MenuPanel::HEIGHT)}, 
      {static_cast<float>(GetScreenWidth()), MenuPanel::HEIGHT}, WHITE
   );

   homeButton.Draw();
   dailyButton.Draw();
   meButton.Draw();
}

Button* MenuPanel::findHoveredButton() {
  if (homeButton.isHovered)
    return &homeButton;
  else if (dailyButton.isHovered)
    return &dailyButton;
  else if (meButton.isHovered)
    return &meButton;
  else
    return nullptr;
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