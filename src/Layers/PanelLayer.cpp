#include <pch/Precompiled.h>
#include "PanelLayer.h"

#include "Core/Application.h"
#include "DailyLayer.h"
#include "GUI/Button.h"
#include "HomeLayer.h"
#include "MeLayer.h"
#include "Storage.h"
#include "Colors.h"

float PanelLayer::buttonSpacing = 0.0f;
int PanelLayer::height = 50;

PanelLayer::PanelLayer() : Core::Layer("Panel Layer", true),
      homeButton(
         { 0, 0 }, { 0, 0 }, "Main", BLANK, Palette::panel_active, 30, { 0.0f, 0 }, Storage::ui.font_semibold),
      dailyButton(
         { 0, 0 }, { 0, 0 }, "Daily Challenges", BLANK, Palette::panel_rest, 30, { 0.0f, 0 }, Storage::ui.font_semibold),
      meButton(
         { 0, 0 }, { 0, 0 }, "Me", BLANK, Palette::panel_rest, 30, { 0.0f, 0 }, Storage::ui.font_semibold)
{
   homeButton.isFocused = true;

   homeButton.setIcon("assets/icons/menus/home_20x20.png");
   dailyButton.setIcon("assets/icons/menus/daily_20x20.png");
   meButton.setIcon("assets/icons/menus/me_20x20.png");

   resize();
}

void PanelLayer::OnAttach() {
   currentLayer = Core::Application::GetLayerByName("Home Layer");
   if(!currentLayer)
      throw std::runtime_error("Panel Layer has no current layer!");

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
         previousButton->setFocus(false, BLANK, Palette::panel_rest);
         activeButton->setFocus(true, BLANK, Palette::panel_active);
         Core::Application::QueueLayerSwap(currentLayer, newLayer);
         currentLayer = newLayer;
         previousButton = activeButton;
      }
   } else if(e.GetEventType() == Core::EventType::WindowResize)
      resize();
}

void PanelLayer::OnUpdate() {
   homeButton.Update();
   dailyButton.Update();
   meButton.Update();

   static GUI::Button* previousButton = &homeButton;
   GUI::Button* hoveredButton = findHoveredButton();

   if(hoveredButton) {
      hoveredButton->contentColor = Palette::panel_hover;
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   }
   // else case should be handled by below layers, since this one is an overlay
   else if(currentLayer->GetName() != "Home Layer") {
      // temp, since rn home layer actually has a working OnUpdate system
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
   }

   if(previousButton && hoveredButton != previousButton) {
      // new button is focused, so we must reset previous one
      if(previousButton->isFocused)
         previousButton->contentColor = Palette::panel_active;
      else
         previousButton->contentColor = Palette::panel_rest;
   }
   previousButton = hoveredButton;
}

void PanelLayer::OnRender() {
   DrawLine(
      0, GetScreenHeight() - height,
      GetScreenWidth(), GetScreenHeight() - height, Palette::shadow_for_bright);
   DrawRectangleV(
      { 0.0f, (float)(GetScreenHeight() - height) },
      { (float)(GetScreenWidth()), (float)height }, Palette::bright_bg);

   homeButton.Draw();
   dailyButton.Draw();
   meButton.Draw();
}

void PanelLayer::resize() {
   // set size
   height = std::max(40, GetScreenHeight() / 15);
   float weirdlyPerfectNumber = height * (5.0f / 7) * 0.9f;  // dont ask why

   homeButton.setFontSize(weirdlyPerfectNumber);
   dailyButton.setFontSize(weirdlyPerfectNumber);
   meButton.setFontSize(weirdlyPerfectNumber);

   // set origins using CSS's space-around formula
   float totalButtonsWidth = homeButton.getSize().x + dailyButton.getSize().x + meButton.getSize().x * 1.3f;  // daily button is huge, me button is tiny, makes the "equal" spacing seem unequal
   float remainingSpace = GetScreenWidth() - totalButtonsWidth;
   float spaceUnit = remainingSpace / (2 * 3);  // each item gets space on left & right, so we divide by twice the no. of items
   buttonSpacing = 2 * spaceUnit;

   float verticalSpace = height - homeButton.getSize().y;

   // |<- (1X) ->[ ITEM 1 ]<- (2X) ->[ ITEM 2 ]<- (2X) ->[ ITEM 3 ]<- (1X) ->|

   homeButton.setOrigin({ spaceUnit, GetScreenHeight() - height + verticalSpace / 2 });
   dailyButton.setOrigin(homeButton.getOrigin() + Vector2{ buttonSpacing + homeButton.getSize().x, 0 });
   meButton.setOrigin(dailyButton.getOrigin() + Vector2{ buttonSpacing + dailyButton.getSize().x, 0 });
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
