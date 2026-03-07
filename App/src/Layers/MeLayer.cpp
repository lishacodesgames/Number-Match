#include <Precompiled.h>
#include "Layers/MeLayer.h"

#include <Button.h>
#include <raylib.h>
#include <Event.h>
#include <cstdio>
#include "Layers/DailyLayer.h"
#include "Layers/HomeLayer.h"
#include "Game.h"

MeLayer::MeLayer() : Layer("MeLayer") { m_panel.meButton.setFocus(true, BLANK, BLUE); }
void MeLayer::OnAttach() { printf("Me Layer attached\n"); }


void MeLayer::OnDetach() { printf("Me Layer detached\n"); }
MeLayer::~MeLayer() = default;

void MeLayer::OnEvent(Event& e) {
   if(e.GetEventType() == EventType::MouseClicked) {
      Button* activeButton = m_panel.findHoveredButton();
      if(!activeButton) {
         e.Handled = false;
         return;
      } else if(activeButton != &m_panel.meButton) {
         Game::Get().QueueLayerPop(this);

         if(activeButton == &m_panel.homeButton)
            Game::Get().QueueLayerPush(new HomeLayer());
         else
            Game::Get().QueueLayerPush(new DailyLayer());
      }

      e.Handled = true;
   }
}

void MeLayer::OnUpdate() { m_panel.Update(&m_panel.meButton); }

void MeLayer::OnRender() {
   DrawText("Coming soon...", GetScreenWidth()/2-150, GetScreenHeight()/2-100, 35, DARKGRAY);
   m_panel.Draw();
}
