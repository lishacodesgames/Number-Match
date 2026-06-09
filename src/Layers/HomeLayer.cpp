#include <pch/Precompiled.h>
#include "HomeLayer.h"

#include "PanelLayer.h"
#include "DailyLayer.h"
#include "GameLayer.h"
#include "CoinLayer.h"
#include "MeLayer.h"
#include "Storage.h"
#include "Colors.h"
#include "App.h"

HomeLayer::HomeLayer() : Layer("Home Layer"),
      m_newButton(
         { 0, 0, 533, 40 },
         "New Game", HOME_BUTTON_1, HOME_BUTTON_2, 30, { 0.8f, 8 }, App::font_semibold
      ),
      m_continueButton(
         { 0, 0, 533, 40 },
         "Continue Game", HOME_BUTTON_2, HOME_BUTTON_1, 30, { 0.8f, 8 }, App::font_semibold
      )
{
   m_backgroundImage = LoadImage("assets/backgrounds/home_background_800x1417.png");
   m_trophyImage = LoadImage("assets/icons/menus/trophy_30x30.png");

   m_backgroundTexture = LoadTextureFromImage(m_backgroundImage);
   m_trophyTexture = LoadTextureFromImage(m_trophyImage);
   
   resize();

   if(!App::GetLayerByName("Coin Layer"))
      App::QueueLayerPush(new CoinLayer());

   Storage::load();
}

void HomeLayer::OnEvent(Core::Event& e) {
   if(e.GetEventType() == Core::EventType::MouseClicked) {
      GUI::Button* activeButton = findHoveredButton();
      if(!activeButton) {
         e.Handled = false;
         return;
      }

      App::QueueLayerPop(App::GetLayerByName("Panel Layer"));
      App::QueueLayerPop(this);

      Core::Layer* game = App::GetLayerByName("Game Layer");

      if(activeButton == &m_continueButton)  // continue pressed & previous game exists
         if(game)
            game->OnResume();
         else
            App::QueueLayerPush(new GameLayer(false));
      else  // new pressed or continue pressed but there was no previous game (hence suspended is false)
         App::QueueLayerPush(new GameLayer(true));

      e.Handled = true;
   }
}

void HomeLayer::OnUpdate() {
   if(IsWindowResized())
      resize();

   m_newButton.Update();
   m_continueButton.Update();

   if(findHoveredButton())
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void HomeLayer::OnRender() {
   DrawTexture(m_backgroundTexture, 0, 0, HOME_BG_OVERLAY);  // background

   // title
   const char* gameName = "Number Match";
   float titleFontSize = std::max(GetScreenHeight() / 10.0f, 45.0f);
   Vector2 titleSize = MeasureTextEx(App::font_black, gameName, titleFontSize, 3);
   Vector2 titleOrigin = {
      (GetScreenWidth() - titleSize.x) / 2.0f,
      GetScreenHeight() / 4.0f
   };

   float shadowOffset = titleFontSize / 15.0f;

   DrawTextEx(App::font_black, gameName, titleOrigin + shadowOffset, titleFontSize, 3.0f, TITLE_SHADOW);
   DrawTextEx(App::font_black, gameName, titleOrigin, titleFontSize, 3.0f, TITLE_COLOR);

   // score
   const char* scoreTag = "All-Time Best Score";
   float scoreTagFontSize = titleFontSize / 2.0f;
   Vector2 scoreTagSize = MeasureTextEx(App::font_semibold, scoreTag, scoreTagFontSize, 1.5f);
   Vector2 scoreTagOrigin = {
      titleOrigin.x + (titleSize.x - scoreTagSize.x) / 1.6f, 
      titleOrigin.y + titleFontSize * 1.05f
   };
   
   DrawTextEx(App::font_semibold, scoreTag, scoreTagOrigin, scoreTagFontSize, 1.5f, GAME_INFO_COLOR);

   float bestScoreFontSize = titleFontSize * 0.7f;
   Vector2 bestScoreSize = MeasureTextEx(App::font_semibold, Storage::formatBestScore().c_str(), bestScoreFontSize, 2.0f);

   Vector2 scoreSize = {
      bestScoreSize.x + m_trophyTexture.width * 1.5f, // half a trophy's width for padding
      std::max((float)m_trophyTexture.height, bestScoreSize.y)
   };
   Vector2 scoreOrigin = {
      scoreTagOrigin.x + (scoreTagSize.x - scoreSize.x) / 2.0f,
      scoreTagOrigin.y + scoreTagSize.y
   };

   Vector2 trophyOrigin = {
      scoreOrigin.x, 
      scoreOrigin.y + (scoreSize.y - m_trophyTexture.height) / 2.0f
   };
   Vector2 bestScoreOrigin = {
      scoreOrigin.x + m_trophyTexture.width * 1.5f,
      scoreOrigin.y - (scoreSize.y - bestScoreSize.y) / 2.0f
   };

   DrawTexture(m_trophyTexture, trophyOrigin.x, trophyOrigin.y, WHITE);
   DrawTextEx(
      App::font_semibold, Storage::formatBestScore().c_str(),
      bestScoreOrigin, bestScoreFontSize, 2.0f, TEXT_FOR_BRIGHT
   );

   // game buttons
   m_newButton.Draw();
   m_continueButton.Draw();
}

void resizeThis(GUI::Button* button) {
   float buttonWidth = std::max(GetScreenWidth() / 2.0f, 400.0f);
   float buttonHeight = std::max(GetScreenHeight() / 20.0f, 40.0f);

   button->setBounds({ buttonWidth, buttonHeight }, true);
}

void HomeLayer::resize() {
   // Buttons
   resizeThis(&m_newButton);
   if(m_newButton.getFontSize() != m_continueButton.getFontSize())
      LOG_RESIZE("Play buttons' font resized to: %d", m_newButton.getFontSize());
   resizeThis(&m_continueButton);

   Vector2 buttonOrigin = {
      (GetScreenWidth() - m_newButton.getSize().x) / 2.0f,
      (GetScreenHeight() - PanelLayer::height - m_newButton.getSize().y * 3.0f)
   };

   m_newButton.setOrigin(buttonOrigin);
   m_continueButton.setOrigin({ buttonOrigin.x, buttonOrigin.y - m_continueButton.getSize().y - 10 });

   // Background
   float aspectRatio = m_backgroundImage.width / (float)m_backgroundImage.height;

   if(GetScreenWidth() > m_backgroundImage.width)
      ImageResize(&m_backgroundImage, GetScreenWidth(), (int)(GetScreenWidth() / aspectRatio));
   else if(GetScreenHeight() > m_backgroundImage.height)
      ImageResize(&m_backgroundImage, (int)(GetScreenHeight() * aspectRatio), GetScreenHeight());

   if(m_backgroundTexture.height != m_backgroundImage.height || m_backgroundTexture.width != m_backgroundImage.width) {
      UnloadTexture(m_backgroundTexture);
      m_backgroundTexture = LoadTextureFromImage(m_backgroundImage);
      LOG_RESIZE("Home background resized to: %d, %d", m_backgroundTexture.width, m_backgroundTexture.height);
   }

   // Best Score's Trophy Icon
   float scale = std::clamp(GetScreenHeight() / 800.0f, 1.0f, 2.5f);
   ImageResize(&m_trophyImage, 30 * scale, 30 * scale); // original trophy size is 30x30
   if(m_trophyTexture.height != m_trophyImage.height) {
      UnloadTexture(m_trophyTexture);
      m_trophyTexture = LoadTextureFromImage(m_trophyImage);
      LOG_RESIZE("Best score icon resized to: %d, %d", m_trophyTexture.width, m_trophyTexture.height);
   }
}