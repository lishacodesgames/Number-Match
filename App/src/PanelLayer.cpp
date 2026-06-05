#include <pch/Precompiled.h>
#include "PanelLayer.h"

#include "DailyLayer.h"
#include "Core/Event.h"
#include "GUI/Button.h"
#include "HomeLayer.h"
#include "MeLayer.h"
#include "App.h"

float PanelLayer::buttonSpacing = 0.0f;
int PanelLayer::height = 50;

PanelLayer::PanelLayer() : Core::Layer("Panel Layer", true),
      homeButton({0, 0}, {0, 0}, "Main", BLANK, BLUE, 30, {0.0f, 0}, App::font_semibold),
      dailyButton({0, 0}, {0, 0}, "Daily Challenges", BLANK, GRAY, 30, {0.0f, 0}, App::font_semibold),
      meButton({0, 0}, {0, 0}, "Me", BLANK, GRAY, 30, {0.0f, 0}, App::font_semibold) 
{
   homeButton.setFocus(true, BLANK, BLUE);

   homeButton.setIcon("assets/icons/menus/home_20x20.png");
   dailyButton.setIcon("assets/icons/menus/daily_20x20.png");
   meButton.setIcon("assets/icons/menus/me_20x20.png");
   
   resize();
}

void PanelLayer::OnAttach() {
   currentLayer = App::GetLayerByName("Home Layer");
   if(!currentLayer)
      TraceLog(LOG_FATAL, "Panel Layer has no current layer!");

   Core::Layer::OnAttach();
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
   if(IsWindowResized())
      resize();

   homeButton.Update();
   dailyButton.Update();
   meButton.Update();

   static GUI::Button* previousButton = &homeButton;
   GUI::Button* hoveredButton = findHoveredButton();

   if (hoveredButton) {
      hoveredButton->setContentColor(DARKBLUE);
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   } // else case will be handled by below layers, since this one is an overlay

   if(previousButton && hoveredButton != previousButton) { // new button is focused, so we must reset previous one
      if(previousButton->isFocused)
         previousButton->setContentColor(BLUE);
      else
         previousButton->setContentColor(GRAY);
   }
   previousButton = hoveredButton;
}

void PanelLayer::OnRender() {
   DrawLine(0, GetScreenHeight() - height, GetScreenWidth(), GetScreenHeight() - height, {180, 180, 180, 255});
   DrawRectangleV(
      {0.0f, (float)(GetScreenHeight() - height)}, 
      {(float)(GetScreenWidth()), (float)height}, WHITE
   );

   homeButton.Draw();
   dailyButton.Draw();
   meButton.Draw();
}

void PanelLayer::resize() {
   // set size
   height = std::max(50, GetScreenHeight() / 15);
   float weirdlyPerfectNumber = height * (5.0f / 7) * 0.9f; // dont ask why

  if((int)weirdlyPerfectNumber != homeButton.getIcon().height) {
      homeButton.setFontSize(weirdlyPerfectNumber);
      TraceLog(LOG_INFO, "RESIZE: Panel \"home\" icon resized to: %d, %d", homeButton.getIcon().width, homeButton.getIcon().height);
      dailyButton.setFontSize(weirdlyPerfectNumber);
      TraceLog(LOG_INFO, "RESIZE: Panel \"daily\" icon resized to: %d, %d", dailyButton.getIcon().width, dailyButton.getIcon().height);
      meButton.setFontSize(weirdlyPerfectNumber);
      TraceLog(LOG_INFO, "RESIZE: Panel \"me\" icon resized to: %d, %d", meButton.getIcon().width, meButton.getIcon().height);
   }

   // set origins using CSS's space-around formula
   float totalButtonsWidth = homeButton.getSize().x + dailyButton.getSize().x + meButton.getSize().x * 1.3f; // daily button is huge, me button is tiny, makes the "equal" spacing seem unequal
   float remainingSpace = GetScreenWidth() - totalButtonsWidth;
   float spaceUnit = remainingSpace/(2*3); // each item gets space on left & right, so we divide by twice the no. of items
   buttonSpacing = 2 * spaceUnit;

   float verticalSpace = height - homeButton.getSize().y;
   
   // |<- (1X) ->[ ITEM 1 ]<- (2X) ->[ ITEM 2 ]<- (2X) ->[ ITEM 3 ]<- (1X) ->|

   homeButton.setOrigin({spaceUnit, GetScreenHeight() - height + verticalSpace / 2});
   dailyButton.setOrigin(homeButton.getOrigin()+ Vector2{buttonSpacing + homeButton.getSize().x, 0});
   meButton.setOrigin(dailyButton.getOrigin() + Vector2{buttonSpacing + dailyButton.getSize().x, 0});
}