#include <Precompiled.h>
#include "MenuPanel.h"

#include <raymath.h>
#include <raylib.h>
#include "Button.h"

static Vector2 buttonsOrigin() { // must be compuled after window exists, hence the function
   return { 
      static_cast<float>(GetScreenWidth() / 3 - 150), 
      static_cast<float>(GetScreenHeight() - MenuPanel::HEIGHT + 15)
   };
}

MenuPanel::MenuPanel() :
      homeButton(
         buttonsOrigin(), {0, 0}, nullptr, "Main", BLANK, BLUE
      ),
      dailyButton(
         buttonsOrigin() + Vector2{BUTTON_SPACING, 0}, {0, 0}, nullptr, "Daily Challenges", BLANK, GRAY
      ),
      meButton(
         buttonsOrigin() + Vector2{BUTTON_SPACING*2.7f, 0}, {0, 0}, nullptr, "Me", BLANK, GRAY
      )
{
   // Load the texture for each button
   int aspectRatio;

   // Home
   Image home = LoadImage("assets/home-icon.png");
   aspectRatio = static_cast<int>(home.width / home.height);
   ImageResize(&home, 20, 20/aspectRatio);

   homeButton.icon = LoadTextureFromImage(home);
   UnloadImage(home);

   // Daily Challenges
   Image daily = LoadImage("assets/daily-icon.png");
   aspectRatio = static_cast<int>(daily.width / daily.height);
   ImageResize(&daily, 20, 20/aspectRatio);

   dailyButton.icon = LoadTextureFromImage(daily);
   UnloadImage(daily);

   // Me
   Image me = LoadImage("assets/me-icon.png");
   aspectRatio = static_cast<int>(me.width / me.height);
   ImageResize(&me, 20, 20/aspectRatio);

   meButton.icon = LoadTextureFromImage(me);
   UnloadImage(me);
}

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
      hoveredButton->contentColor = DARKBLUE;
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   } else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void MenuPanel::Draw() {
   DrawLine(0, GetScreenHeight() - HEIGHT, GetScreenWidth(), GetScreenHeight() - HEIGHT, {180, 180, 180, 255});
   DrawRectangleV(
      {0.0f, static_cast<float>(GetScreenHeight() - HEIGHT)}, 
      {static_cast<float>(GetScreenWidth()), HEIGHT}, WHITE
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