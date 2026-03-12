#include <Precompiled.h>
#include "Layers/PanelLayer.h"

#include <raymath.h>
#include <raylib.h>
#include "Layers/DailyLayer.h"
#include "Layers/HomeLayer.h"
#include "Layers/MeLayer.h"
#include "Button.h"
#include "Event.h"
#include "App.h"

static Vector2 buttonsOrigin() { // must be compuled after window exists, hence the function
   return { 
      static_cast<float>(GetScreenWidth() / 3 - 180),
      static_cast<float>(GetScreenHeight() - PanelLayer::HEIGHT + 10)
   };
}

PanelLayer* PanelLayer::s_instance = nullptr;
void PanelLayer::PopInstance() {
   if(s_instance)
      App::QueueLayerPop(s_instance);
}

PanelLayer::PanelLayer() : Layer("Panel Layer", true),
      homeButton(
         {0, 0}, {0, 0}, "Main", BLANK, BLUE, 30, {0.0f, 0}, App::font_semibold
      ),
      dailyButton(
         {0, 0}, {0, 0}, "Daily Challenges", BLANK, GRAY, 30, {0.0f, 0}, App::font_semibold
      ),
      meButton(
         {0, 0}, {0, 0}, "Me", BLANK, GRAY, 30, {0.0f, 0}, App::font_semibold
      ) 
{
   s_instance = this;
   homeButton.setFocus(true, BLANK, BLUE);

   homeButton.setOrigin(buttonsOrigin());
   dailyButton.setOrigin(homeButton.getOrigin() + Vector2{BUTTON_SPACING, 0});
   meButton.setOrigin(homeButton.getOrigin() + Vector2{BUTTON_SPACING * 2.7f, 0});

   homeButton.setIcon("assets/icons/menus/home_20x20.png");
   dailyButton.setIcon("assets/icons/menus/daily_20x20.png");
   meButton.setIcon("assets/icons/menus/me_20x20.png");
}
PanelLayer::~PanelLayer() { s_instance = nullptr; }

void PanelLayer::OnEvent(Event& e) {
   if(e.GetEventType() == EventType::MouseClicked) {
      Button* activeButton = findHoveredButton();
      if(!activeButton)
         return;

      Layer* newLayer = nullptr;
      if(activeButton == &homeButton && currentPage != Menu::Home) {
         newLayer = new HomeLayer();
         currentPage = Menu::Home;
      } else if(activeButton == &dailyButton && currentPage != Menu::Daily) {
         newLayer = new DailyLayer();
         currentPage = Menu::Daily;
      } else if(activeButton == &meButton && currentPage != Menu::Me) {
         newLayer = new MeLayer();
         currentPage = Menu::Me;
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
