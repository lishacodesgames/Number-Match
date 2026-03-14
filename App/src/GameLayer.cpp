#include <pch/Precompiled.h>
#include "GameLayer.h"

#include <raymath.h>
#include <raylib.h>
#include "OptionsLayer.h"
#include "PanelLayer.h"
#include "HomeLayer.h"
#include "Core/Logging.h"
#include "Core/Layer.h"
#include "App.h"

GameLayer::GameLayer() : Core::Layer("Game Layer"),
      m_gobackButton({15, 15}, {0, 0}, "", BLANK, Color{42, 187, 235, 255}, 20, {0, 0}),
      m_settingsButton(
         {static_cast<float>(GetScreenWidth()) - 45, 15}, {0, 0}, "", BLANK, Color{42, 187, 235, 255}, 20, {0, 0}
      ),
      m_plusButton(
         {static_cast<float>(GetScreenWidth()) / 2 - 12, static_cast<float>(GetScreenHeight()) - 40}, // oiginr
         {5, 5}, "", {102, 191, 255, 200}, BLUE, 25, {1.0f, 8} // cyanish bg, circle
      )
{      
   m_gobackButton.setIcon("assets/icons/game/goback_18x24.png");
   m_settingsButton.setIcon("assets/icons/game/settings_30x30.png");
   m_plusButton.setIcon("assets/icons/game/plus_24x24.png");
}

void GameLayer::OnAttach() {
   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
   Core::Layer::OnAttach();   
}

void GameLayer::OnEvent(Core::Event& e) {
   if(isSuspended && !suspended_event)
      return;

   if(e.GetEventType() == Core::EventType::KeyPressed) {
      char key = static_cast<Core::KeyPressedEvent&>(e).key;
      if(key == 'q' || key == 'Q') {
         OnSuspend();
         App::QueueLayerPush(new HomeLayer());
         App::QueueLayerPush(new PanelLayer());
         e.Handled = true;
      }
   } else if(e.GetEventType() == Core::EventType::MouseClicked) {
      GUI::Button* activeButton = findHoveredButton();

      if(activeButton == &m_gobackButton) {
         OnSuspend();
         App::QueueLayerPush(new HomeLayer());
         App::QueueLayerPush(new PanelLayer());
         e.Handled = true;
      } else if(activeButton == &m_settingsButton) {
         OnSuspend(true); // suspend but render
         App::QueueLayerPush(new OptionsLayer());
         e.Handled = true;
      } else if (activeButton == &m_plusButton) {
         TraceLog(LISHA_SAYS, "PLUS"); // temp
      }
   }
}

void GameLayer::OnUpdate() {
   if(isSuspended && !suspended_update)
      return;
   
   m_gobackButton.Update();
   m_settingsButton.Update();
   m_plusButton.Update();

   if(findHoveredButton())
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void GameLayer::OnRender() {
   if(isSuspended && !suspended_render)
      return;

   m_gobackButton.Draw();
   m_settingsButton.Draw();
   m_plusButton.Draw();
}

#pragma region Helpers

GUI::Button* GameLayer::findHoveredButton() {
   if(m_gobackButton.isHovered)
      return &m_gobackButton;
   else if(m_settingsButton.isHovered)
      return &m_settingsButton;
   else if(m_plusButton.isHovered)
      return &m_plusButton;
   else
      return nullptr;
}

#pragma endregion