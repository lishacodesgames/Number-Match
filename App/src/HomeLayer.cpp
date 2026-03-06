#include <Precompiled.h>
#include "HomeLayer.h"

#include <raymath.h>
#include <raylib.h>
#include <cstdio>
#include "DailyLayer.h"
#include "GameLayer.h"
#include "MenuPanel.h"
#include "MeLayer.h"
#include "Game.h"

/// @bug if clicked super quick, start button will not be active & layer will not transition

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
   // start of frame: update
   m_panel.Update();
   m_startButton.Update();
   
   // panel focus
   m_panel.clearAllFocus();
   m_focusedPanelButton->setFocus(true, BLANK, BLUE);

   // hover state of all Menu buttons
   Button* hoveredButton = findHoveredButton();
   if(hoveredButton) {
      if(hoveredButton != &m_startButton)
         hoveredButton->textColor = DARKBLUE;
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   } else 
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
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