#include <pch/Precompiled.h>
#include "HomeLayer.h"

#include <raymath.h>
#include <raylib.h>
#include "PanelLayer.h"
#include "DailyLayer.h"
#include "GameLayer.h"
#include "MeLayer.h"
#include "App.h"

static constexpr Vector2 buttonBounds = {350, 35};
static Vector2 buttonOrigin() {
   return {
      static_cast<float>(GetScreenWidth() - buttonBounds.x)/2, 
      static_cast<float>(GetScreenHeight() - PanelLayer::HEIGHT - buttonBounds.y - 50)
   };
}
HomeLayer::HomeLayer() : Layer("Home Layer"), 
      m_newButton(
         {buttonOrigin().x, buttonOrigin().y, buttonBounds.x, buttonBounds.y}, 
         "New Game", WHITE, BLUE, 25, {0.8f, 8}, App::font_semibold
      ),
      m_continueButton(
         {buttonOrigin().x, buttonOrigin().y-buttonBounds.y-10, buttonBounds.x, buttonBounds.y}, 
         "Continue Game", BLUE, WHITE, 25, {0.8f, 8}, App::font_semibold
      )
{
   m_backgroundTexture = LoadTexture("assets/backgrounds/home_background_800x1417.jpg");

   m_coinTexture = LoadTexture("assets/icons/game/coin_20x20.png");
   m_trophyTexture = LoadTexture("assets/icons/menus/trophy_30x30.png");
}
HomeLayer::~HomeLayer() { UnloadTexture(m_backgroundTexture); }

void HomeLayer::OnEvent(Core::Event &e) {
   if(e.GetEventType() == Core::EventType::MouseClicked) {
      GUI::Button* activeButton = findHoveredButton();
      if(!activeButton) {
         e.Handled = false;
         return;
      }
      
      PanelLayer::PopInstance();    
      App::QueueLayerPop(this);

      Core::Layer* game = App::GetLayerByName("Game Layer");
      if(activeButton == &m_continueButton && game) // continue pressed & previous game exists
         game->OnResume();
      else // new pressed or continue pressed but there was no previous game (hence suspended is false) 
         App::QueueLayerPush(new GameLayer());

      e.Handled = true;
   }
}

void HomeLayer::OnUpdate() {
   m_newButton.Update();
   m_continueButton.Update();

   if(findHoveredButton())
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

Vector2 operator+(const Vector2& vec, const float& fl) { return {vec.x+fl, vec.y+fl}; }

void HomeLayer::OnRender() {
   DrawTexture(m_backgroundTexture, 0, 0, {255, 255, 255, 30}); // background

   // coins display. @todo add changeability
   Rectangle coinBox = {static_cast<float>(GetScreenWidth())/2-48, 20, 100, 40};
   DrawRectangleRounded(coinBox, 5.0f, 5, WHITE);
   DrawTexture(m_coinTexture, coinBox.x+10.0f, coinBox.y+10.0f, WHITE);
   DrawTextEx(App::font_semibold, "1,343", {coinBox.x+34.0f, coinBox.y+5.85f}, 27.3f, 1.5f, {0, 0, 0, 175});

   // title
   const char* gameName = "Number Match";
   Vector2 titleOrigin = {
      static_cast<float>(GetScreenWidth() - MeasureTextEx(App::font_black, gameName, 45, 1).x)/2 - 60, 
      150.0f
   };
   DrawTextEx(App::font_black, gameName, titleOrigin+2.0f, 60, 3.0f, {3, 21, 41, 255}); // outline
   DrawTextEx(App::font_black, gameName, titleOrigin, 60, 3.0f, DARKBLUE);

   // score
   DrawTextEx(
      App::font_semibold, "All-Time Best Score", {titleOrigin.x+90, titleOrigin.y+58}, 23, 1.5f, {125, 125, 125, 255}
   );
   DrawTexture(m_trophyTexture, titleOrigin.x+105, titleOrigin.y+84, WHITE);
   DrawTextEx(App::font_semibold, "32,582", {titleOrigin.x+140, titleOrigin.y+78}, 40, 2.0f, {60, 60, 60, 255});
   
   // game buttons
   m_newButton.Draw();
   m_continueButton.Draw();
}

GUI::Button* HomeLayer::findHoveredButton() {
   if(m_newButton.isHovered)
      return &m_newButton;
   else if(m_continueButton.isHovered)
      return &m_continueButton;
   else
      return nullptr;
}