#include <Precompiled.h>
#include "MenuLayer.h"

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
   DrawTexture(m_backgroundTexture, 0, 0, {240, 240, 240, 150}); // background

   DrawText("Welcome to the Game!", 172, 152, 45, BLACK); // outline
   DrawText("Welcome to the Game!", 170, 150, 45, DARKBLUE);
   m_startButton.Draw();
}
