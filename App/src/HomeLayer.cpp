#include <pch/Precompiled.h>
#include "HomeLayer.h"

#include "PanelLayer.h"
#include "DailyLayer.h"
#include "GameLayer.h"
#include "CoinLayer.h"
#include "MeLayer.h"
#include "Colors.h"
#include "Storage.h"
#include "App.h"

HomeLayer::HomeLayer() : Layer("Home Layer"),
      m_newButton(
         { 0, 0, 533, 40 },
         "New Game", WHITE, BLUE, 30, { 0.8f, 8 }, App::font_semibold
      ),
      m_continueButton(
         { 0, 0, 533, 40 },
         "Continue Game", BLUE, WHITE, 30, { 0.8f, 8 }, App::font_semibold
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
   DrawTexture(m_backgroundTexture, 0, 0, ColorAlpha(WHITE, 0.16897f));  // background

   // title
   const char* gameName = "Number Match";
   float titleFontSize = std::clamp(GetScreenHeight() / 10.0f, 45.0f, 70.0f);
   Vector2 titleSize = MeasureTextEx(App::font_black, gameName, titleFontSize, 3);
   Vector2 titleOrigin = {
      static_cast<float>(GetScreenWidth() - titleSize.x) / 2,
      GetScreenHeight() / 4.0f
   };

   constexpr Color SHADOW = { 3, 21, 41, 255 };
   float shadowOffset = titleFontSize / 15.0f;

   DrawTextEx(App::font_black, gameName, titleOrigin + shadowOffset, titleFontSize, 3.0f, SHADOW);
   DrawTextEx(App::font_black, gameName, titleOrigin, titleFontSize, 3.0f, DARKBLUE);

   // score
   const char* scoreTag = "All-Time Best Score";
   float scoreTagFontSize = titleFontSize / 2.0f;
   Vector2 scoreTagSize = MeasureTextEx(App::font_semibold, scoreTag, scoreTagFontSize, 1.5f);
   Vector2 scoreTagOrigin = {
      titleOrigin.x + (titleSize.x - scoreTagSize.x) / 1.6f, 
      titleOrigin.y + titleFontSize * 1.05f
   };
   
   DrawTextEx(App::font_semibold, scoreTag, scoreTagOrigin, scoreTagFontSize, 1.5f, MEDIUMDARKGRAY);

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
      bestScoreOrigin, bestScoreFontSize, 2.0f, DARKERGRAY
   );

   // game buttons
   m_newButton.Draw();
   m_continueButton.Draw();
}

void resizeThis(GUI::Button* button) {
   Vector2 buttonBounds = {
      std::clamp(GetScreenWidth() / 1.5f, 400.0f, 550.0f),
      std::clamp(GetScreenHeight() / 20.0f, 40.0f, 60.0f)
   };

   button->setBounds(buttonBounds, true);
}

void HomeLayer::resize() {
   // Buttons
   resizeThis(&m_newButton);
   if(m_newButton.getFontSize() != m_continueButton.getFontSize())
      TraceLog(LOG_INFO, "Play button font resized to: %d", m_newButton.getFontSize());
   resizeThis(&m_continueButton);

   Vector2 buttonOrigin = {
      static_cast<float>(GetScreenWidth() - m_newButton.getSize().x) / 2,
      static_cast<float>(GetScreenHeight() - PanelLayer::height - m_newButton.getSize().y - 50)
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
      TraceLog(LOG_INFO, "RESIZE: Home background resized to: %d, %d", m_backgroundTexture.width, m_backgroundTexture.height);
   }

   // Best Score's Trophy Icon
   float scale = std::clamp(GetScreenHeight() / 800.0f, 1.0f, 2.5f);
   ImageResize(&m_trophyImage, 30 * scale, 30 * scale); // original trophy size is 30x30
   if(m_trophyTexture.height != m_trophyImage.height) {
      UnloadTexture(m_trophyTexture);
      m_trophyTexture = LoadTextureFromImage(m_trophyImage);
      TraceLog(LOG_INFO, "RESIZE: Best score icon resized to: %d, %d", m_trophyTexture.width, m_trophyTexture.height);
   }
}