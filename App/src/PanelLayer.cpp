#include <pch/Precompiled.h>
#include "PanelLayer.h"

#include "Core/Logging.h"
#include "DailyLayer.h"
#include "Core/Event.h"
#include "GUI/Button.h"
#include "HomeLayer.h"
#include "MeLayer.h"
#include "App.h"

static Vector2 buttonsOrigin() { // must be compuled after window exists, hence the function
   return { 
      static_cast<float>(GetScreenWidth() / 3 - 180),
      static_cast<float>(GetScreenHeight() - PanelLayer::HEIGHT + 10)
   };
}

Menu PanelLayer::currentPage = Menu::None;

PanelLayer::PanelLayer() : Core::Layer("Panel Layer", true),
      homeButton({0, 0}, {0, 0}, "Main", BLANK, BLUE, 30, {0.0f, 0}, App::font_semibold),
      dailyButton({0, 0}, {0, 0}, "Daily Challenges", BLANK, GRAY, 30, {0.0f, 0}, App::font_semibold),
      meButton({0, 0}, {0, 0}, "Me", BLANK, GRAY, 30, {0.0f, 0}, App::font_semibold) 
{
   homeButton.setFocus(true, BLANK, BLUE);
   currentPage = Menu::Home;

   setButtonsOrigin();

   homeButton.setIcon("assets/icons/menus/home_20x20.png");
   dailyButton.setIcon("assets/icons/menus/daily_20x20.png");
   meButton.setIcon("assets/icons/menus/me_20x20.png");
}

void PanelLayer::OnEvent(Core::Event& e) {
   if(e.GetEventType() == Core::EventType::MouseClicked) {
      GUI::Button* activeButton = findHoveredButton();
      if(!activeButton)
         return;

      Core::Layer* newLayer = nullptr;
      if(activeButton == &homeButton && currentPage != Menu::Home) {
         currentPage = Menu::Home;
         newLayer = new HomeLayer();
      } else if(activeButton == &dailyButton && currentPage != Menu::Daily) {
         currentPage = Menu::Daily;
         newLayer = new DailyLayer();
      } else if(activeButton == &meButton && currentPage != Menu::Me) {
         currentPage = Menu::Me;
         newLayer = new MeLayer();
      }

      if(newLayer) {
         resetAllFocus();
         activeButton->setFocus(true, BLANK, BLUE);
         App::QueueLayerSwap(currentLayer, newLayer);
         currentLayer = newLayer;
      }
   }
}

void PanelLayer::OnUpdate() {
   if(IsWindowResized()) {
      setButtonsOrigin();
      TraceLog(LOG_INFO, "Window Resized: %d, %d", GetScreenWidth(), GetScreenHeight());
   }

   homeButton.Update();
   dailyButton.Update();
   meButton.Update();

   /// @todo fix logic with previousCell, like in Grid::Update()
   GUI::Button* focusedButton = findFocusedButton();
   resetAllFocus();
   focusedButton->setFocus(true, BLANK, BLUE);

   GUI::Button* hoveredButton = findHoveredButton();
   if (hoveredButton) {
      hoveredButton->contentColor = DARKBLUE;
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   } // else case will be handled by below layers, since this one is an overlay
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

GUI::Button* PanelLayer::findHoveredButton() {
   if (homeButton.isHovered)
      return &homeButton;
   else if (dailyButton.isHovered)
      return &dailyButton;
   else if (meButton.isHovered)
      return &meButton;
   else
      return nullptr;
}

GUI::Button* PanelLayer::findFocusedButton() {
   if(homeButton.isFocused)
      return &homeButton;
   else if(dailyButton.isFocused)
      return &dailyButton;
   else if(meButton.isFocused)
      return &meButton;
   else {
      TraceLog(LOG_ERROR, "No panel button is focused!");
      return nullptr;
   }
}

void PanelLayer::resetAllFocus() {
   homeButton.setFocus(false, BLANK, GRAY);
   dailyButton.setFocus(false, BLANK, GRAY);
   meButton.setFocus(false, BLANK, GRAY);
}

void PanelLayer::setButtonsOrigin() {
   homeButton.setOrigin(buttonsOrigin());
   dailyButton.setOrigin(homeButton.getOrigin() + Vector2{BUTTON_SPACING, 0});
   meButton.setOrigin(homeButton.getOrigin() + Vector2{BUTTON_SPACING * 2.7f, 0});
}
