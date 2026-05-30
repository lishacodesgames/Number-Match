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

PanelLayer::PanelLayer() : Core::Layer("Panel Layer", true),
      homeButton({0, 0}, {0, 0}, "Main", BLANK, BLUE, 30, {0.0f, 0}, App::font_semibold),
      dailyButton({0, 0}, {0, 0}, "Daily Challenges", BLANK, GRAY, 30, {0.0f, 0}, App::font_semibold),
      meButton({0, 0}, {0, 0}, "Me", BLANK, GRAY, 30, {0.0f, 0}, App::font_semibold) 
{
   homeButton.setFocus(true, BLANK, BLUE);

   setButtonsOrigin();

   homeButton.setIcon("assets/icons/menus/home_20x20.png");
   dailyButton.setIcon("assets/icons/menus/daily_20x20.png");
   meButton.setIcon("assets/icons/menus/me_20x20.png");
}

void PanelLayer::OnEvent(Core::Event& e) {
   if(e.GetEventType() == Core::EventType::MouseClicked) {
      GUI::Button* activeButton = findHoveredButton();
      if(!activeButton || activeButton->isFocused)
         return;

      static GUI::Button* previousButton = &homeButton;
      Core::Layer* newLayer = nullptr;
      if(activeButton != previousButton) {
         if(activeButton == &homeButton)
            newLayer = new HomeLayer();
         else if(activeButton == &dailyButton)
            newLayer = new DailyLayer();
         else if(activeButton == &meButton)
            newLayer = new MeLayer();
      }
      if(newLayer) {
         previousButton->setFocus(false, BLANK, GRAY);
         activeButton->setFocus(true, BLANK, BLUE);
         App::QueueLayerSwap(currentLayer, newLayer);
         currentLayer = newLayer;
         previousButton = activeButton;
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

   static GUI::Button* previousButton = &homeButton;
   GUI::Button* hoveredButton = findHoveredButton();

   if (hoveredButton) {
      hoveredButton->contentColor = DARKBLUE;
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   } // else case will be handled by below layers, since this one is an overlay

   if(previousButton && hoveredButton != previousButton) { // new button is focused, so we must reset previous one
      if(previousButton->isFocused)
         previousButton->contentColor = BLUE;
      else
         previousButton->contentColor = GRAY;
   }
   previousButton = hoveredButton;
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

void PanelLayer::setButtonsOrigin() {
   homeButton.setOrigin(buttonsOrigin());
   dailyButton.setOrigin(homeButton.getOrigin() + Vector2{BUTTON_SPACING, 0});
   meButton.setOrigin(homeButton.getOrigin() + Vector2{BUTTON_SPACING * 2.7f, 0});
}