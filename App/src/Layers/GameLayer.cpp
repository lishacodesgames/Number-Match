#include <Precompiled.h>
#include "Layers/GameLayer.h"

#include <raylib.h>
#include "Layers/PanelLayer.h"
#include "Layers/GameLayer.h"
#include "Layers/HomeLayer.h"
#include "Layer.h"
#include "App.h"

GameLayer::GameLayer() : Layer("Game Layer") {}

void GameLayer::OnAttach() {
   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
   Layer::OnAttach();   
}

void GameLayer::OnUpdate() {
   if(isSuspended && !suspended_update)
      return;

  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) x += 4;
  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) x -= 4;
  if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) y -= 4;
  if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) y += 4;
}

void GameLayer::OnEvent(Event& e) {
   if(isSuspended && !suspended_event)
      return;

   if(e.GetEventType() == EventType::KeyPressed) {
      char key = static_cast<KeyPressedEvent&>(e).key;
      if(key == 'q' || key == 'Q') {
         OnSuspend();
         App::QueueLayerPush(new HomeLayer());
         App::QueueLayerPush(new PanelLayer());
         e.Handled = true;
      }
   }
}

void GameLayer::OnRender() {
   if(isSuspended && !suspended_render)
      return;

   Vector2 GamePos = {static_cast<float>(GetScreenWidth() / 2 - 70), 50};
   DrawTextEx(App::font_black, "GAME", GamePos, 70, 5, DARKBLUE);
   DrawTextEx(App::font_semibold, "Press Q to return to menu", {GamePos.x-25, GamePos.y+70}, 22, 1.5f, GRAY);
   DrawTextEx(App::font_semibold, "Move circle with WASD or arrows", {GamePos.x-150, GamePos.y+90}, 40, 2, DARKGRAY);

   DrawCircle(x, y, 25, PINK);
}
