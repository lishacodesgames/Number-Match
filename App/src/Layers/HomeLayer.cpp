#include <Precompiled.h>
#include "Layers/HomeLayer.h"

#include <raymath.h>
#include <raylib.h>
#include <cstdio>
#include "Layers/DailyLayer.h"
#include "Layers/GameLayer.h"
#include "MenuPanel.h"
#include "Layers/MeLayer.h"
#include "Game.h"

HomeLayer::HomeLayer() : Layer("HomeLayer") {
   Image bg = LoadImage("assets/home_background.jpg");
   if(bg.data != nullptr) {
      ImageResize(&bg, GetScreenWidth(), GetScreenHeight() - MenuPanel::HEIGHT);
      m_backgroundTexture = LoadTextureFromImage(bg);
      UnloadImage(bg);
   }

   m_startButton = Button({ 320, 250 }, { 22, 14 }, "Start the Game", 22, PINK, DARKGRAY);
   m_focusedPanelButton = &m_panel.homeButton;
}
void HomeLayer::OnAttach() { printf("Menu Layer attached\n"); }

void HomeLayer::OnDetach() { printf("Menu Layer detached\n"); }
HomeLayer::~HomeLayer() { UnloadTexture(m_backgroundTexture); }

void HomeLayer::OnEvent(Event &e) {
   if(e.GetEventType() == EventType::MouseClicked) {
      Button* activeButton = findHoveredButton();
      if(!activeButton) {
         e.Handled = false;
         return;
      }
      else if(activeButton == &m_startButton)
         Game::Get().QueueLayerSwap(this, new GameLayer());
      else if(activeButton == &m_panel.dailyButton)
         Game::Get().QueueLayerSwap(this, new DailyLayer());
      else if(activeButton == &m_panel.meButton)
         Game::Get().QueueLayerSwap(this, new MeLayer());

      e.Handled = true;
   }
}

void HomeLayer::OnUpdate() {
   m_panel.Update(&m_panel.homeButton);
   m_startButton.Update();

   if(m_startButton.isHovered)
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
}

void HomeLayer::OnRender() {
   DrawTexture(m_backgroundTexture, 0, 0, {255, 255, 255, 60});

   DrawText("Welcome to the Game!", 172, 152, 45, BLACK); // outline
   DrawText("Welcome to the Game!", 170, 150, 45, DARKBLUE);
   m_startButton.Draw();

   m_panel.Draw();
}

Button* HomeLayer::findHoveredButton() {
   if(m_startButton.isHovered)
      return &m_startButton;
   else 
      return m_panel.findHoveredButton();
}