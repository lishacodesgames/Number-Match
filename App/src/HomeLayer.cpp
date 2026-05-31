#include <pch/Precompiled.h>
#include "HomeLayer.h"

#include "PanelLayer.h"
#include "DailyLayer.h"
#include "GameLayer.h"
#include "CoinLayer.h"
#include "MeLayer.h"
#include "Storage.h"
#include "App.h"

HomeLayer::HomeLayer() : Layer("Home Layer"), 
      m_newButton(
         {0, 0}, {0, 0},
         "New Game", WHITE, BLUE, 25, {0.8f, 8}, App::font_semibold
      ),
      m_continueButton(
         {0, 0}, {0, 0},
         "Continue Game", BLUE, WHITE, 25, {0.8f, 8}, App::font_semibold
      )
{
   resizeButtons();

   m_backgroundImage = LoadImage("assets/backgrounds/home_background_800x1417.png"); 
   resizeBackground();
   m_backgroundTexture = LoadTextureFromImage(m_backgroundImage);
   m_trophyTexture = LoadTexture("assets/icons/menus/trophy_30x30.png");

   if(!App::GetLayerByName("Coin Layer"))
      App::QueueLayerPush(new CoinLayer());

   Storage::load();
}

void HomeLayer::OnEvent(Core::Event &e) {
   if(e.GetEventType() == Core::EventType::MouseClicked) {
      GUI::Button* activeButton = findHoveredButton();
      if(!activeButton) {
         e.Handled = false;
         return;
      }
      
      App::QueueLayerPop(App::GetLayerByName("Panel Layer"));
      App::QueueLayerPop(this);

      Core::Layer* game = App::GetLayerByName("Game Layer");

      if(activeButton == &m_continueButton) // continue pressed & previous game exists
         if(game)
            game->OnResume();
         else
            App::QueueLayerPush(new GameLayer(false));
      else // new pressed or continue pressed but there was no previous game (hence suspended is false) 
         App::QueueLayerPush(new GameLayer(true));

      e.Handled = true;
   }
}

void HomeLayer::OnUpdate() {
   if(IsWindowResized()) {
      resizeBackground();
      resizeButtons();
   }

   m_newButton.Update();
   m_continueButton.Update();

   if(findHoveredButton())
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void HomeLayer::OnRender() {
   DrawTexture(m_backgroundTexture, 0, 0, {255, 255, 255, 30}); // background

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
   DrawTextEx(
      App::font_semibold, Storage::formatBestScore().c_str(), 
      {titleOrigin.x + 140, titleOrigin.y + 78}, 40, 2.0f, {60, 60, 60, 255}
   );
   
   // game buttons
   m_newButton.Draw();
   m_continueButton.Draw();
}

void setSize(GUI::Button* button) {
   Vector2 buttonBounds = {
      std::clamp(GetScreenWidth() / 1.5f, 400.0f, 700.0f),
      std::clamp(GetScreenHeight() / 20.0f, 40.0f, 60.0f)
   };

   button->setSize(buttonBounds);
   button->fontSize = buttonBounds.y * 0.7f;
}

void HomeLayer::resizeButtons() {
   // set size
   setSize(&m_newButton);
   setSize(&m_continueButton);

   // set origin
   Vector2 buttonOrigin = {
      static_cast<float>(GetScreenWidth() - m_newButton.getSize().x)/2, 
      static_cast<float>(GetScreenHeight() - PanelLayer::height - m_newButton.getSize().y - 50)
   };

   m_newButton.setOrigin(buttonOrigin);
   m_continueButton.setOrigin({buttonOrigin.x, buttonOrigin.y - m_continueButton.getSize().y - 10});
}

void HomeLayer::resizeBackground() {
   float aspectRatio = m_backgroundImage.width / (float)m_backgroundImage.height;

   if(GetScreenWidth() > m_backgroundImage.width)
      ImageResize(&m_backgroundImage, GetScreenWidth(), (int)(GetScreenWidth() / aspectRatio));
   else if(GetScreenHeight() > m_backgroundImage.height)
      ImageResize(&m_backgroundImage, (int)(GetScreenHeight() * aspectRatio), GetScreenHeight());

   UnloadTexture(m_backgroundTexture);
   m_backgroundTexture = LoadTextureFromImage(m_backgroundImage);
}