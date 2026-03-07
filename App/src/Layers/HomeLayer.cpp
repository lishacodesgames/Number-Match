#include <Precompiled.h>
#include "Layers/HomeLayer.h"

#include <raymath.h>
#include <raylib.h>
#include <cstdio>
#include "Layers/DailyLayer.h"
#include "Layers/GameLayer.h"
#include "Layers/MeLayer.h"
#include "MenuPanel.h"
#include "Game.h"

static constexpr Vector2 buttonBounds = {350, 40};
static Vector2 buttonOrigin() {
   return {
      static_cast<float>(GetScreenWidth() - buttonBounds.x)/2, 
      static_cast<float>(GetScreenHeight() - MenuPanel::HEIGHT - buttonBounds.y - 50)
   };
}
HomeLayer::HomeLayer() : Layer("Home Layer"), 
      m_newButton({buttonOrigin().x, buttonOrigin().y, buttonBounds.x, buttonBounds.y}, "New Game", WHITE, BLUE),
      m_continueButton({buttonOrigin().x, buttonOrigin().y-buttonBounds.y-15, buttonBounds.x, buttonBounds.y}, "Continue Game", BLUE, WHITE)
{
   Image bg = LoadImage("assets/home_background.jpg");
   if(bg.data != nullptr) {
      ImageResize(&bg, GetScreenWidth(), bg.height*1.5); // resize only width so it extends below screen
      m_backgroundTexture = LoadTextureFromImage(bg);
      UnloadImage(bg);
   }

   m_focusedPanelButton = &m_panel.homeButton;
}
HomeLayer::~HomeLayer() { UnloadTexture(m_backgroundTexture); }

void HomeLayer::OnEvent(Event &e) {
   if(e.GetEventType() == EventType::MouseClicked) {
      Button* activeButton = findHoveredButton();
      if(!activeButton) {
         e.Handled = false;
         return;
      } else if(activeButton != &m_panel.homeButton) {
         App::Get().QueueLayerPop(this);

         if(activeButton == &m_panel.dailyButton)
            App::Get().QueueLayerPush(new DailyLayer()); 
         else if(activeButton == &m_panel.meButton)
            App::Get().QueueLayerPush(new MeLayer());
         else if(activeButton == &m_newButton)
            App::Get().QueueLayerPush(new GameLayer());
         else {// resume game layer
            if(GameLayer::s_isSuspended)        // game layer exists and was suspended
               GameLayer::s_isSuspended = false;
            else                                // game layer was not created before this
               App::Get().QueueLayerPush(new GameLayer());
         }
      }
      
      e.Handled = true;
   }
}

void HomeLayer::OnUpdate() {
   m_panel.Update(&m_panel.homeButton);
   m_newButton.Update();
   m_continueButton.Update();

   if(m_newButton.isHovered || m_continueButton.isHovered)
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
}

void HomeLayer::OnRender() {
   DrawTexture(m_backgroundTexture, 0, 0, {255, 255, 255, 25});

   const char* gameName = "Number Match";
   Vector2 textPos = {
      static_cast<float>(GetScreenWidth() - MeasureTextEx(GetFontDefault(), gameName, 45, 1).x)/2 - 20, 
      150.0f
   };
   DrawText(gameName, textPos.x + 2, textPos.y + 2, 45, BLACK); // outline
   DrawText(gameName, textPos.x, textPos.y, 45, DARKBLUE);
   
   m_newButton.Draw();
   m_continueButton.Draw();
   m_panel.Draw();
}

Button* HomeLayer::findHoveredButton() {
   if(m_newButton.isHovered)
      return &m_newButton;
   else if(m_continueButton.isHovered)
      return &m_continueButton;
   else 
      return m_panel.findHoveredButton();
}