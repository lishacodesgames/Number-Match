#include <Precompiled.h>
#include "MenuLayer.h"

#include <raymath.h>
#include <raylib.h>
#include <cstdio>
#include "GameLayer.h"
#include "Game.h"

MenuLayer::MenuLayer() : Layer("MenuLayer") {
   Image bg = LoadImage("assets/background.jpg");
   ImageResize(&bg, GetScreenWidth(), GetScreenHeight());
   m_backgroundTexture = LoadTextureFromImage(bg);
   UnloadImage(bg);

   m_startButton = Button({ 320, 250 }, { 22, 14 }, "Start the Game", 22, PINK, DARKGRAY);

   constexpr float PANEL_HEIGHT = 50.0f;
   constexpr float BUTTON_SPACING = 192.0f;
   Vector2 panelButtonsOrigin = { 
      (float)(GetScreenWidth()) / 3 - 150, (float)(GetScreenHeight()) - PANEL_HEIGHT + 15
   };
   m_homeButton = Button( panelButtonsOrigin, {0, 0}, "Main", BLANK, BLUE);
   m_dailyButton = Button( panelButtonsOrigin + Vector2{BUTTON_SPACING, 0}, {0, 0}, "Daily Challenges", BLANK, GRAY);
   m_meButton = Button( panelButtonsOrigin + Vector2{BUTTON_SPACING*2.7f, 0}, {0, 0}, "Me", BLANK, GRAY);
}

MenuLayer::~MenuLayer() {
   UnloadTexture(m_backgroundTexture);
}

void MenuLayer::OnAttach() {
   printf("Menu Layer attached\n");
}

void MenuLayer::OnDetach() {
   printf("Menu Layer detached\n");
}

void MenuLayer::OnUpdate() {
   if(m_startButton.isHovering()) {
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   }
   else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
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
   DrawTexture(m_backgroundTexture, 0, 0, {255, 255, 255, 150}); // background
   
   // panel
   DrawLine(0, GetScreenHeight() - 50, GetScreenWidth(), GetScreenHeight() - 50, DARKGRAY);
   DrawRectangleV(
      {0, (float)(GetScreenHeight() - 50)}, {(float)GetScreenWidth(), 50}, WHITE
   );
   m_homeButton.Draw();
   m_dailyButton.Draw();
   m_meButton.Draw();

   DrawText("Welcome to the Game!", 172, 152, 45, BLACK); // outline
   DrawText("Welcome to the Game!", 170, 150, 45, DARKBLUE);
   m_startButton.Draw();
}
