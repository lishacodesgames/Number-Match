#include <Precompiled.h>
#include "MenuPanel.h"

#include <raymath.h>
#include <raylib.h>
#include "Button.h"
#include "Event.h"

static Vector2 buttonsOrigin() { // must be compuled after window exists, hence the function
   return { 
      static_cast<float>(GetScreenWidth() / 3 - 150), 
      static_cast<float>(GetScreenHeight() - PanelLayer::HEIGHT + 15)
   };
}

PanelLayer::PanelLayer() : Layer("Panel Layer"),
      homeButton(
         {0, 0}, {0, 0}, nullptr, "Main", BLANK, BLUE
      ),
      dailyButton(
         {0, 0}, {0, 0}, nullptr, "Daily Challenges", BLANK, GRAY
      ),
      meButton(
         {0, 0}, {0, 0}, nullptr, "Me", BLANK, GRAY
      )
{
   homeButton.origin = buttonsOrigin();
   dailyButton.origin = homeButton.origin + Vector2{BUTTON_SPACING, 0};
   meButton.origin = homeButton.origin + Vector2{BUTTON_SPACING * 2.7f, 0};

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

void PanelLayer::OnEvent(Event& e) {
   // TODO
}

void PanelLayer::OnUpdate() {
  homeButton.Update();
  dailyButton.Update();
  meButton.Update();

  Button* focusedButton = findFocusedButton();
  resetAllFocus();
  focusedButton->setFocus(true, BLANK, BLUE);

  Button* hoveredButton = findHoveredButton();
  if (hoveredButton) {
    hoveredButton->contentColor = DARKBLUE;
    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
  } else
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void PanelLayer::OnRender() {
   DrawLine(0, GetScreenHeight() - HEIGHT, GetScreenWidth(), GetScreenHeight() - HEIGHT, {180, 180, 180, 255});
   DrawRectangleV(
      {0.0f, static_cast<float>(GetScreenHeight() - HEIGHT)}, 
      {static_cast<float>(GetScreenWidth()), HEIGHT}, WHITE
   );

   homeButton.Draw();
   dailyButton.Draw();
   meButton.Draw();
}

Button* PanelLayer::findHoveredButton() {
  if (homeButton.isHovered)
    return &homeButton;
  else if (dailyButton.isHovered)
    return &dailyButton;
  else if (meButton.isHovered)
    return &meButton;
  else
    return nullptr;
}

Button* PanelLayer::findActiveButton() {
   if(homeButton.isActive)
      return &homeButton;
   else if(dailyButton.isActive)
      return &dailyButton;
   else if(meButton.isActive)
      return &meButton;
   else  
      return nullptr;
}

Button* PanelLayer::findFocusedButton() {
   if(homeButton.isFocused)
      return &homeButton;
   else if(dailyButton.isFocused)
      return &dailyButton;
   else if(meButton.isFocused)
      return &meButton;
   else {
      TraceLog(LOG_ERROR, "LISHA SAYS: No panel button is focused!");
      return nullptr;
   }
 }

 void PanelLayer::resetAllFocus() {
   homeButton.setFocus(false, BLANK, GRAY);
   dailyButton.setFocus(false, BLANK, GRAY);
   meButton.setFocus(false, BLANK, GRAY);
 }
