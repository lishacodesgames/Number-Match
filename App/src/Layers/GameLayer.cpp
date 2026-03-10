#include <Precompiled.h>
#include "Layers/GameLayer.h"

#include <raymath.h>
#include <raylib.h>
#include "Layers/PanelLayer.h"
#include "Layers/GameLayer.h"
#include "Layers/HomeLayer.h"
#include "Layer.h"
#include "App.h"

GameLayer::GameLayer() : Layer("Game Layer"),
      gobackButton({15, 15}, {0, 0}, nullptr, "", BLANK, Color{42, 187, 235, 255}, 20, {0, 0}) // cyan
{ gobackButton.setIcon("assets/icons/goback_24x24.png"); }

void GameLayer::OnAttach() {
   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
   Layer::OnAttach();   
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
   } else if(e.GetEventType() == EventType::MouseClicked && gobackButton.isHovered) {
      OnSuspend();
      App::QueueLayerPush(new HomeLayer());
      App::QueueLayerPush(new PanelLayer());
      e.Handled = true;
   }
}

void GameLayer::OnUpdate() {
   if(isSuspended && !suspended_update)
      return;
   
   gobackButton.Update();
   if(gobackButton.isHovered)
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void GameLayer::OnRender() {
   if(isSuspended && !suspended_render)
      return;

   gobackButton.Draw();
}
