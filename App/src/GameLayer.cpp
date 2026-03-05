#include <Precompiled.h>
#include "GameLayer.h"

#include <Layer.h>
#include <cstdio>
#include "MenuLayer.h"
#include "Game.h"

GameLayer::GameLayer() : Layer("GameLayer") {}

void GameLayer::OnAttach() {
   printf("Game Layer attached\n");
}

void GameLayer::OnDetach() {
   printf("Game Layer detached\n");
}

void GameLayer::OnUpdate() {
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) x += 4;
  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) x -= 4;
  if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) y -= 4;
  if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) y += 4;
}

void GameLayer::OnEvent(Event& e) {
   if(e.GetEventType() == EventType::KeyPressed) {
      char key = static_cast<KeyPressedEvent&>(e).key;
      if(key == 'q' || key == 'Q') {
         Game::Get().QueueLayerSwap(this, new MenuLayer());
         e.Handled = true;
      }
   }
}

void GameLayer::OnRender() {
   DrawText("GAME", GetScreenWidth() / 2 - 100, 50, 55, DARKBLUE);
   DrawText("Move circle with WASD or arrows", GetScreenWidth() / 2 - 250, 120, 30, DARKGRAY);
   DrawText("Press Q to return to menu", GetScreenWidth() / 2 - 120, 150, 20, GRAY);

   DrawCircle(x, y, 25, PINK);
}
