#include <Precompiled.h>
#include "Layers/DailyLayer.h"

#include <raylib.h>
#include <Event.h>
#include <Layer.h>
#include <cstdio>
#include "Layers/HomeLayer.h"
#include "Layers/MeLayer.h"
#include "Game.h"

DailyLayer::DailyLayer() : Layer("Daily Layer") { m_panel.dailyButton.setFocus(true, BLANK, BLUE); }

void DailyLayer::OnEvent(Event& e) {
   if(e.GetEventType() == EventType::MouseClicked) {
      Button* activeButton = m_panel.findHoveredButton();
      if(!activeButton) {
         e.Handled = false;
         return;
      } else if (activeButton != &m_panel.dailyButton) {
         App::Get().QueueLayerPop(this);

         if (activeButton == &m_panel.homeButton)
            App::Get().QueueLayerPush(new HomeLayer());
         else
            App::Get().QueueLayerPush(new MeLayer());
      }
      
      e.Handled = true;
   }
}

void DailyLayer::OnUpdate() { m_panel.Update(&m_panel.dailyButton); }

void DailyLayer::OnRender() {
   DrawText("Coming soon...", GetScreenWidth()/2-150, GetScreenHeight()/2-100, 35, DARKGRAY);
   m_panel.Draw();
}