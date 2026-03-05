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

   Vector2 panelButtonsOrigin = { 
      (float)(GetScreenWidth() / 3 - 150), (float)(GetScreenHeight() - 35) 
   };
   m_Home = Button( panelButtonsOrigin, {0, 0}, "Main", BLANK, BLUE);
   m_Daily = Button( panelButtonsOrigin + Vector2{192, 0}, {0, 0}, "Daily Challenges", BLANK, GRAY);
   m_Me = Button( panelButtonsOrigin + Vector2{520, 0}, {0, 0}, "Me", BLANK, GRAY);
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

   DrawText("Welcome to the Game!", 172, 152, 45, BLACK); // outline
   DrawText("Welcome to the Game!", 170, 150, 45, DARKBLUE);
   m_startButton.Draw();

   // panel
   DrawLine(0, GetScreenHeight() - 50, GetScreenWidth(), GetScreenHeight() - 50, DARKGRAY);
   DrawRectangleV(
      {0, (float)(GetScreenHeight() - 50)}, {(float)GetScreenWidth(), 50}, WHITE
   );
   m_Home.Draw();
   m_Daily.Draw();
   m_Me.Draw();
}
