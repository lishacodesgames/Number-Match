#include <pch/Precompiled.h>
#include "PanelLayer.h"

#include "Core/Logging.h"
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
      resize();
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
   DrawLine(0, GetScreenHeight() - height, GetScreenWidth(), GetScreenHeight() - height, {180, 180, 180, 255});
   DrawRectangleV(
      {0.0f, (float)(GetScreenHeight() - height)}, 
      {(float)(GetScreenWidth()), (float)height}, WHITE
   );

   homeButton.Draw();
   dailyButton.Draw();
   meButton.Draw();
}

void scaleThisButtonToBeTaller(GUI::Button* button, float height) {
   float sizeRatio = button->getSize().x / button->getSize().y;
   button->resize({height * sizeRatio, height});
}

void PanelLayer::resize() {
   // set size
   height = std::max(50, GetScreenHeight() / 15);
   scaleThisButtonToBeTaller(&homeButton, height * 5.5f/7);
   scaleThisButtonToBeTaller(&dailyButton, height * 5.5f/7);
   scaleThisButtonToBeTaller(&meButton, height * 5.5f/7);

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