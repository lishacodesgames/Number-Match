#include <Precompiled.h>
#include "MenuLayer.h"

#include <raymath.h>
#include <raylib.h>
#include <cstdio>
#include "GameLayer.h"
#include "Game.h"

/// @bug if clicked super quick, start button will not be active & layer will not transition

MenuLayer::MenuLayer() : Layer("MenuLayer") {
   Image bg = LoadImage("assets/background.jpg");
   if(bg.data != nullptr) {
      ImageResize(&bg, GetScreenWidth(), GetScreenHeight() - MenuPanel::HEIGHT);
      m_backgroundTexture = LoadTextureFromImage(bg);
      UnloadImage(bg);
   }

   m_startButton = Button({ 320, 250 }, { 22, 14 }, "Start the Game", 22, PINK, DARKGRAY);
   m_focusedPanelButton = &m_panel.homeButton;
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
   // panel
   m_panel.homeButton.Update();
   m_panel.dailyButton.Update();
   m_panel.meButton.Update();

   m_panel.homeButton.setFocus(false, BLANK, GRAY);
   m_panel.dailyButton.setFocus(false, BLANK, GRAY);
   m_panel.meButton.setFocus(false, BLANK, GRAY);

   Button* activePanelButton = m_panel.findActiveButton();
   if(activePanelButton != nullptr && activePanelButton != m_focusedPanelButton)
      m_focusedPanelButton = activePanelButton;

   m_focusedPanelButton->setFocus(true, BLANK, BLUE);

   Button* hoveredButton = m_panel.findHoveredButton();
   if(hoveredButton) {
      hoveredButton->textColor = DARKBLUE;
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   } else 
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);

   // start button
   m_startButton.Update();
   if(m_startButton.isHovered)
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
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
   DrawLine(0, GetScreenHeight() - MenuPanel::HEIGHT, GetScreenWidth(), GetScreenHeight() - MenuPanel::HEIGHT, DARKGRAY);
   DrawRectangleV(
      {0.0f, (float)(GetScreenHeight() - MenuPanel::HEIGHT)}, 
      {(float)GetScreenWidth(), MenuPanel::HEIGHT}, WHITE
   );
   m_panel.homeButton.Draw();
   m_panel.dailyButton.Draw();
   m_panel.meButton.Draw();

   DrawText("Welcome to the Game!", 172, 152, 45, BLACK); // outline
   DrawText("Welcome to the Game!", 170, 150, 45, DARKBLUE);
   m_startButton.Draw();
}

Button* MenuPanel::findHoveredButton() {
   if(homeButton.isHovered)
      return &homeButton;
   else if(dailyButton.isHovered)
      return &dailyButton;
   else if(meButton.isHovered)
      return &meButton;
   else  
      return nullptr;
}

Button* MenuPanel::findActiveButton() {
   if(homeButton.isActive)
      return &homeButton;
   else if(dailyButton.isActive)
      return &dailyButton;
   else if(meButton.isActive)
      return &meButton;
   else  
      return nullptr;
}

MenuPanel::MenuPanel() {
   constexpr float BUTTON_SPACING = 192.0f;
   Vector2 panelButtonsOrigin = { 
      (float)(GetScreenWidth()) / 3 - 150, (float)(GetScreenHeight()) - MenuPanel::HEIGHT + 15
   };

   homeButton = Button( panelButtonsOrigin, {0, 0}, "Main", BLANK, BLUE);
   dailyButton = Button( panelButtonsOrigin + Vector2{BUTTON_SPACING, 0}, {0, 0}, "Daily Challenges", BLANK, GRAY);
   meButton = Button( panelButtonsOrigin + Vector2{BUTTON_SPACING*2.7f, 0}, {0, 0}, "Me", BLANK, GRAY);
}
