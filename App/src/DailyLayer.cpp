#include <Precompiled.h>
#include "DailyLayer.h"

#include <raylib.h>
#include <Event.h>
#include <Layer.h>
#include <cstdio>
#include "HomeLayer.h"
#include "Game.h"

/// @bug Trophy texture not loading

DailyLayer::DailyLayer() : Layer("DailyLayer") {
   TraceLog(LOG_INFO, "Working Directory: %s", GetWorkingDirectory());
   Image bg = LoadImage("assets/daily_trophy.jpeg");
   if(bg.data != nullptr) {
      ImageResize(&bg, GetScreenWidth(), GetScreenHeight());
      m_trophyTexture = LoadTextureFromImage(bg);
      UnloadImage(bg);
   }

   m_focusedPanelButton = &m_panel.dailyButton;
}
void DailyLayer::OnAttach() { 
   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
   printf("DailyLayer attached\n"); 
}

void DailyLayer::OnDetach() { printf("DailyLayer detached\n"); }
DailyLayer::~DailyLayer() { UnloadTexture(m_trophyTexture); }

void DailyLayer::OnEvent(Event& e) {
   if(e.GetEventType() == EventType::MouseClicked) {
      Button* activeButton = m_panel.findHoveredButton();
      if(!activeButton) {
         e.Handled = false;
         return;
      }
      else if(activeButton == &m_panel.homeButton)
         Game::Get().QueueLayerSwap(this, new HomeLayer());
      
      e.Handled = true;
   }
}

void DailyLayer::OnUpdate() {
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

void DailyLayer::OnRender() {
   DrawTexture(m_trophyTexture, 0, 0, {255, 255, 255, 150});
   DrawText("Coming soon...", GetScreenWidth()/2-150, GetScreenHeight()/2-100, 35, DARKGRAY);
   m_panel.Draw();
}