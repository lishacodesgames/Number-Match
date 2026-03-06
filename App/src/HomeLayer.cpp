#include <Precompiled.h>
#include "HomeLayer.h"

#include <raymath.h>
#include <raylib.h>
#include <cstdio>
#include "MenuPanel.h"
#include "GameLayer.h"
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

HomeLayer::~HomeLayer() {
   if(IsTextureValid(m_backgroundTexture))
      UnloadTexture(m_backgroundTexture);
}

void HomeLayer::OnAttach() {
   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
   printf("Menu Layer attached\n");
}

void HomeLayer::OnDetach() {
   printf("Menu Layer detached\n");
}

void HomeLayer::OnEvent(Event &e) {
   if(e.GetEventType() == EventType::MouseClicked) {
      Button* activeButton = findHoveredButton();
      if(!activeButton) {
         e.Handled = false;
         return;
      }
      else if(activeButton == &m_startButton)
         Game::Get().QueueLayerSwap(this, new GameLayer());
      else if(activeButton != m_focusedPanelButton)
         m_focusedPanelButton = activeButton;

      e.Handled = true;
   }
}

void HomeLayer::OnUpdate() {
   // start of frame: update
   m_panel.homeButton.Update();
   m_panel.dailyButton.Update();
   m_panel.meButton.Update();
   m_startButton.Update();
   
   // panel: active & focus
   m_panel.homeButton.setFocus(false, BLANK, GRAY);
   m_panel.dailyButton.setFocus(false, BLANK, GRAY);
   m_panel.meButton.setFocus(false, BLANK, GRAY);

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
   else if(m_panel.homeButton.isHovered)
      return &m_panel.homeButton;
   else if(m_panel.dailyButton.isHovered)
      return &m_panel.dailyButton;
   else if(m_panel.meButton.isHovered)
      return &m_panel.meButton;
   else  
      return nullptr;
}