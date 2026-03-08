#include <Precompiled.h>
#include "Layers/HomeLayer.h"

#include <raymath.h>
#include <raylib.h>
#include "Layers/PanelLayer.h"
#include "Layers/DailyLayer.h"
#include "Layers/GameLayer.h"
#include "Layers/MeLayer.h"
#include "App.h"

static constexpr Vector2 buttonBounds = {350, 40};
static Vector2 buttonOrigin() {
   return {
      static_cast<float>(GetScreenWidth() - buttonBounds.x)/2, 
      static_cast<float>(GetScreenHeight() - PanelLayer::HEIGHT - buttonBounds.y - 50)
   };
}
HomeLayer::HomeLayer() : Layer("Home Layer", false), 
      m_newButton(
         {buttonOrigin().x, buttonOrigin().y, buttonBounds.x, buttonBounds.y}, 
         nullptr, "New Game", WHITE, BLUE
      ),
      m_continueButton(
         {buttonOrigin().x, buttonOrigin().y-buttonBounds.y-15, buttonBounds.x, buttonBounds.y}, 
         nullptr, "Continue Game", BLUE, WHITE
      )
{
   Image bg = LoadImage("assets/home_background.jpg");
   if(bg.data) {
      ImageResize(&bg, GetScreenWidth(), bg.height*1.5); // resize only width so it extends below screen
      m_backgroundTexture = LoadTextureFromImage(bg);
      UnloadImage(bg);
   }

   Image coin = LoadImage("assets/coin-icon.png");
   if(coin.data) {
      ImageResize(&coin, 15, 15);
      m_coinTexture = LoadTextureFromImage(coin);
      UnloadImage(coin);
   }

   Image trophy = LoadImage("assets/trophy-icon.png");
   if(trophy.data) {
      ImageResize(&trophy, 27, 27);
      m_trophyTexture = LoadTextureFromImage(trophy);
      UnloadImage(trophy);
   }
}
HomeLayer::~HomeLayer() { UnloadTexture(m_backgroundTexture); }

void HomeLayer::OnEvent(Event &e) {
   if(e.GetEventType() == EventType::MouseClicked) {
      Button* activeButton = findHoveredButton();
      if(!activeButton) {
         e.Handled = false;
         return;
      }
      
      PanelLayer::PopInstance();         
      App::Get().QueueLayerPop(this);
      if(activeButton == &m_continueButton && GameLayer::s_isSuspended)
         GameLayer::setSuspended(false);
      else // new pressed or continue pressed but there was no previous game (hence suspended is false) 
         App::Get().QueueLayerPush(new GameLayer());

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

void HomeLayer::OnRender() {
   DrawTexture(m_backgroundTexture, 0, 0, {255, 255, 255, 30}); // background

   // coins display. TODO add changeability
   Rectangle coinBox = {static_cast<float>(GetScreenWidth())/2-48, 20, 100, 40};
   DrawRectangleRounded(coinBox, 5.0f, 5, WHITE);
   DrawTexture(m_coinTexture, coinBox.x+11.5f, coinBox.y+11.5f, WHITE);
   DrawText(      "1,343",    coinBox.x+37.0f, coinBox.y+11.5f, 20, {0, 0, 0, 175});

   // title
   const char* gameName = "Number Match";
   Vector2 titleOrigin = {
      static_cast<float>(GetScreenWidth() - MeasureTextEx(GetFontDefault(), gameName, 45, 1).x)/2 - 20, 
      150.0f
   };
   DrawText(gameName, titleOrigin.x + 2, titleOrigin.y + 2, 45, BLACK); // outline
   DrawText(gameName, titleOrigin.x, titleOrigin.y, 45, DARKBLUE);

   // score
   DrawText("All-Time Best Score", titleOrigin.x+78, titleOrigin.y+58, 18, {100, 100, 100, 255});
   DrawTexture(m_trophyTexture, titleOrigin.x+78, titleOrigin.y+84, WHITE);
   DrawText("32,582", titleOrigin.x+122, titleOrigin.y+84, 35, DARKGRAY);
   
   // game buttons
   m_newButton.Draw();
   m_continueButton.Draw();
}

Button* HomeLayer::findHoveredButton() {
   if(m_newButton.isHovered)
      return &m_newButton;
   else if(m_continueButton.isHovered)
      return &m_continueButton;
   else
      return nullptr;
}