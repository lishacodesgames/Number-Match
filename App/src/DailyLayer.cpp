#include <Precompiled.h>
#include "DailyLayer.h"

#include <raylib.h>
#include <Event.h>
#include <Layer.h>
#include <cstdio>
#include "HomeLayer.h"
#include "MeLayer.h"
#include "Game.h"

DailyLayer::DailyLayer() : Layer("DailyLayer") { m_panel.dailyButton.setFocus(true, BLANK, BLUE); }
void DailyLayer::OnAttach() { printf("Daily Layer attached\n"); }

void DailyLayer::OnDetach() { printf("Daily Layer detached\n"); }
DailyLayer::~DailyLayer() = default;

void DailyLayer::OnEvent(Event& e) {
   if(e.GetEventType() == EventType::MouseClicked) {
      Button* activeButton = m_panel.findHoveredButton();
      if(!activeButton) {
         e.Handled = false;
         return;
      }
      else if(activeButton == &m_panel.homeButton)
         Game::Get().QueueLayerSwap(this, new HomeLayer());
      else if(activeButton == &m_panel.meButton)
         Game::Get().QueueLayerSwap(this, new MeLayer());
      
      e.Handled = true;
   }
}

void DailyLayer::OnUpdate() {
   m_panel.Update();
   
   m_panel.clearAllFocus();
   m_panel.dailyButton.setFocus(true, BLANK, BLUE);

   Button* hoveredButton = m_panel.findHoveredButton();
   if(hoveredButton) {
      hoveredButton->textColor = DARKBLUE;
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   } else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void DailyLayer::OnRender() {
   DrawText("Coming soon...", GetScreenWidth()/2-150, GetScreenHeight()/2-100, 35, DARKGRAY);
   m_panel.Draw();
}