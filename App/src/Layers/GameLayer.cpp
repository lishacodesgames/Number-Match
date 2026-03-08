#include <Precompiled.h>
#include "Layers/GameLayer.h"

#include <raylib.h>
#include "Layers/PanelLayer.h"
#include "Layers/GameLayer.h"
#include "Layers/HomeLayer.h"
#include "Layer.h"
#include "App.h"

bool GameLayer::s_isSuspended = false;
void GameLayer::setSuspended(bool state) {
   // TODO FIX TO CALL OnSuspend() and OnResume()
   if(s_isSuspended == state)
      return;
   
   s_isSuspended = state;
   if(s_isSuspended)
      TraceLog(LOG_INFO, "LISHA SAYS: Game Layer SUSPENDED");
   else
      TraceLog(LOG_INFO, "LISHA SAYS: Game Layer RESUMED");
}

GameLayer::GameLayer() : Layer("Game Layer", false) {
  renderSuspended = false;
  GameLayer::s_isSuspended = false;
}
void GameLayer::OnAttach() {
   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
   TraceLog(LOG_INFO, "LISHA SAYS: %s ATTACHED", m_name.c_str());
}

void GameLayer::OnUpdate() {
   if(s_isSuspended)
      return;

  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) x += 4;
  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) x -= 4;
  if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) y -= 4;
  if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) y += 4;
}

void GameLayer::OnEvent(Event& e) {
   if(s_isSuspended)
      return;

   if(e.GetEventType() == EventType::KeyPressed) {
      char key = static_cast<KeyPressedEvent&>(e).key;
      if(key == 'q' || key == 'Q') {
         setSuspended(true);
         App::Get().QueueLayerPush(new HomeLayer());
         App::Get().QueueLayerPush(new PanelLayer());
         e.Handled = true;
      }
   }
}

void GameLayer::OnRender() {
   if(s_isSuspended && !renderSuspended)
      return;

   DrawText("GAME", GetScreenWidth() / 2 - 100, 50, 55, DARKBLUE);
   DrawText("Move circle with WASD or arrows", GetScreenWidth() / 2 - 250, 120, 30, DARKGRAY);
   DrawText("Press Q to return to menu", GetScreenWidth() / 2 - 120, 150, 20, GRAY);

   DrawCircle(x, y, 25, PINK);
}
