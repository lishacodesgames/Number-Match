#include <Precompiled.h>
#include "MeLayer.h"

#include <raylib.h>
#include <Event.h>
#include <cstdio>
#include "DailyLayer.h"
#include "HomeLayer.h"
#include "Game.h"

MeLayer::MeLayer() : Layer("MeLayer") { m_focusedPanelButton = &m_panel.meButton; }
void MeLayer::OnAttach() { printf("Me Layer attached\n"); }


void MeLayer::OnDetach() { printf("Me Layer detached\n"); }
MeLayer::~MeLayer() = default;

void MeLayer::OnEvent(Event& e) {
   if(e.GetEventType() == EventType::MouseClicked) {
      Button* activeButton = m_panel.findHoveredButton();
      if(!activeButton) {
         e.Handled = false;
         return;
      } 
      else if(activeButton == &m_panel.homeButton)
         Game::Get().QueueLayerSwap(this, new HomeLayer());
      else if(activeButton == &m_panel.dailyButton)
         Game::Get().QueueLayerSwap(this, new DailyLayer());

      e.Handled = true;
   }
}

void MeLayer::OnUpdate() {
   m_panel.Update();
   
   m_panel.clearAllFocus();
   m_focusedPanelButton->setFocus(true, BLANK, BLUE);

   Button* hoveredButton = m_panel.findHoveredButton();
   if(hoveredButton) {
      hoveredButton->textColor = BLUE;
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   } else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void MeLayer::OnRender() {
   DrawText("Coming soon...", GetScreenWidth()/2-150, GetScreenHeight()/2-100, 35, DARKGRAY);
   m_panel.Draw();
}
