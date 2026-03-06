#include <Precompiled.h>
#include "MenuLayer.h"

#include <raymath.h>
#include <raylib.h>
#include <cstdio>
#include "GameLayer.h"
#include "Game.h"

/// @bug if clicked super quick, start button will not be active & layer will not transition
/// @bug Active buttons not rendering

static constexpr float PANEL_HEIGHT = 50.0f;
MenuLayer::MenuLayer() : Layer("MenuLayer") {

   Image bg = LoadImage("assets/background.jpg");
   if(bg.data != nullptr) {
      ImageResize(&bg, GetScreenWidth(), GetScreenHeight() - PANEL_HEIGHT);
      m_backgroundTexture = LoadTextureFromImage(bg);
      UnloadImage(bg);
   }

   m_startButton = Button({ 320, 250 }, { 22, 14 }, "Start the Game", 22, PINK, DARKGRAY);

   constexpr float BUTTON_SPACING = 192.0f;
   Vector2 panelButtonsOrigin = { 
      (float)(GetScreenWidth()) / 3 - 150, (float)(GetScreenHeight()) - PANEL_HEIGHT + 15
   };
   m_homeButton = Button( panelButtonsOrigin, {0, 0}, "Main", BLANK, BLUE);
   m_dailyButton = Button( panelButtonsOrigin + Vector2{BUTTON_SPACING, 0}, {0, 0}, "Daily Challenges", BLANK, GRAY);
   m_meButton = Button( panelButtonsOrigin + Vector2{BUTTON_SPACING*2.7f, 0}, {0, 0}, "Me", BLANK, GRAY);
}

MenuLayer::~MenuLayer() {
   if(IsTextureValid(m_backgroundTexture))
      UnloadTexture(m_backgroundTexture);
}

void MenuLayer::OnAttach() {
   printf("Menu Layer attached\n");
}

void MenuLayer::OnDetach() {
   printf("Menu Layer detached\n");
}

void MenuLayer::OnUpdate() {
   m_startButton.Update();
   m_homeButton.Update();
   m_dailyButton.Update();
   m_meButton.Update();

   m_homeButton.setFocus(false, BLANK, GRAY);
   m_dailyButton.setFocus(false, BLANK, GRAY);
   m_meButton.setFocus(false, BLANK, GRAY);

   Button* activeButton = m_findActiveButton();
   if(activeButton && activeButton != &m_startButton)
      (*activeButton).setFocus(true, BLANK, BLUE);
   else if(!activeButton)
      m_homeButton.setFocus(true, BLANK, BLUE);

   Button* hoveredButton = m_findHoveredButton();
   if(hoveredButton) {
      if(*hoveredButton != m_startButton)
         (*hoveredButton).textColor = DARKBLUE;
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   } else 
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void MenuLayer::OnEvent(Event &e) {
   if(e.GetEventType() == EventType::MouseClicked) {
      if(m_startButton.isClicked()) {
         Game::Get().QueueLayerSwap(this, new GameLayer());
         e.Handled = true;
      }
   }
}

void MenuLayer::OnRender() {
   // background
   DrawTexture(m_backgroundTexture, 0, 0, {255, 255, 255, 60});
   
   // panel
   DrawLine(0, GetScreenHeight() - PANEL_HEIGHT, GetScreenWidth(), GetScreenHeight() - PANEL_HEIGHT, DARKGRAY);
   DrawRectangleV(
      {0.0f, (float)(GetScreenHeight() - PANEL_HEIGHT)}, 
      {(float)GetScreenWidth(), PANEL_HEIGHT}, WHITE
   );
   m_homeButton.Draw();
   m_dailyButton.Draw();
   m_meButton.Draw();

   DrawText("Welcome to the Game!", 172, 152, 45, BLACK); // outline
   DrawText("Welcome to the Game!", 170, 150, 45, DARKBLUE);
   m_startButton.Draw();
}

Button* MenuLayer::m_findHoveredButton() {
   if(m_startButton.isHovered)
      return &m_startButton;
   else if(m_homeButton.isHovered)
      return &m_homeButton;
   else if(m_dailyButton.isHovered)
      return &m_dailyButton;
   else if(m_meButton.isHovered)
      return &m_meButton;
   else  
      return nullptr;
}

Button* MenuLayer::m_findActiveButton() {
   if(m_startButton.isActive)
      return &m_startButton;
   else if(m_homeButton.isActive)
      return &m_homeButton;
   else if(m_dailyButton.isActive)
      return &m_dailyButton;
   else if(m_meButton.isActive)
      return &m_meButton;
   else  
      return nullptr;
}
