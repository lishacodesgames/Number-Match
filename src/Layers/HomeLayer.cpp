#include <pch/Precompiled.h>
#include "HomeLayer.h"

#include "QuestionLayer.h"
#include "PanelLayer.h"
#include "GameLayer.h"
#include "CoinLayer.h"
#include "Storage.h"
#include "Colors.h"
#include "App.h"

HomeLayer::HomeLayer() : Layer("Home Layer"),
      m_backgroundTexture(LoadTexture("assets/backgrounds/home_background_800x1417.png")),
      m_trophyTexture(LoadTexture("assets/icons/menus/trophy_30x30.png")),
      m_newButton(
         { 0, 0, 533, 40 }, "New Game", Palette::home_button_1,
         Palette::home_button_2, 30, { 0.8f, 8 }, App::font_semibold),
      m_continueButton(
         { 0, 0, 533, 40 }, "Continue Game", Palette::home_button_2,
         Palette::home_button_1, 30, { 0.8f, 8 }, App::font_semibold)
{
   resize();

   if(!App::GetLayerByName("Coin Layer"))
      App::QueueLayerPush(new CoinLayer());
}

#pragma region Methods

void HomeLayer::OnEvent(Core::Event& e) {
   if(e.GetEventType() == Core::EventType::MouseClicked) {
      GUI::Button* activeButton = findHoveredButton();
      if(!activeButton)
         return;

      Core::Layer* game = App::GetLayerByName("Game Layer");
      Core::Layer* panel = App::GetLayerByName("Panel Layer");
      if(!panel)
         throw std::runtime_error("Home Layer exists but Panel Layer doesn't!");

      if(activeButton == &m_newButton) {
         panel->OnSuspend(true);

         App::QueueLayerPush(new QuestionLayer("Start a new game?\nYour previous game progress will be lost.",
            [this, panel](bool yes) {
               if(!yes) {
                  panel->OnResume();
                  return;
               }

               App::QueueLayerPop(panel);
               App::QueueLayerSwap(this, new GameLayer(true));
            }, this));
      } else { // continue button was pressed
         App::QueueLayerPop(panel);
         App::QueueLayerPop(this);

         if(game)
            game->OnResume();
         else  // game layer doesn't exist but we mustn't reset player progress
            App::QueueLayerPush(new GameLayer(false));
      }

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
   DrawTextureEx(m_backgroundTexture, { 0, 0 }, 0.0f, m_bgScale, Palette::home_bg_overlay);

   // title
   const char* gameName = "Number Match";
   float titleFontSize =
      std::max(std::min(GetScreenHeight(), GetScreenWidth()) / 10.0f, 45.0f);
   Vector2 titleSize = MeasureTextEx(App::font_black, gameName, titleFontSize, 3);
   Vector2 titleOrigin = { (GetScreenWidth() - titleSize.x) / 2.0f, GetScreenHeight() / 4.0f };

   DrawTextEx(
      App::font_black, gameName, titleOrigin + titleFontSize / 15.0f,
      titleFontSize, 3.0f, Palette::title_shadow);
   DrawTextEx(
      App::font_black, gameName, titleOrigin,
      titleFontSize, 3.0f, Palette::title_color);

   // score
   const char* scoreTag = "All-Time Best Score";
   Vector2 scoreTagSize = MeasureTextEx(App::font_semibold, scoreTag, titleFontSize / 2.0f, 1.5f);
   Vector2 scoreTagOrigin = {
      titleOrigin.x + (titleSize.x - scoreTagSize.x) / 1.6f, 
      titleOrigin.y + titleFontSize * 1.05f };
   
   DrawTextEx(
      App::font_semibold, scoreTag, scoreTagOrigin,
      titleFontSize / 2.0f, 1.5f, Palette::game_info_color);

   float bestScoreFontSize = titleFontSize * 0.7f;
   Vector2 bestScoreSize = MeasureTextEx(App::font_semibold, Storage::format(Storage::game.bestScore).c_str(), bestScoreFontSize, 2.0f);

   Vector2 scoreSize = {
      bestScoreSize.x + m_trophyScale * m_trophyTexture.width * 1.5f, // half a trophy's width for padding
      std::max(m_trophyScale * m_trophyTexture.height, bestScoreSize.y) };
   Vector2 scoreOrigin = {
      scoreTagOrigin.x + (scoreTagSize.x - scoreSize.x) / 2.0f,
      scoreTagOrigin.y + scoreTagSize.y };

   Vector2 trophyOrigin = {
      scoreOrigin.x, scoreOrigin.y + (scoreSize.y - m_trophyScale * m_trophyTexture.height) / 2.0f };
   Vector2 bestScoreOrigin = {
      scoreOrigin.x + m_trophyScale * m_trophyTexture.width * 1.5f,
      scoreOrigin.y - (scoreSize.y - bestScoreSize.y) / 2.0f };

   DrawTextureEx(m_trophyTexture, trophyOrigin, 0.0f, m_trophyScale, WHITE);
   DrawTextEx(
      App::font_semibold, Storage::format(Storage::game.bestScore).c_str(),
      bestScoreOrigin, bestScoreFontSize, 2.0f, Palette::text_for_bright);

   // game buttons
   m_newButton.Draw();
   m_continueButton.Draw();
}

#pragma endregion

#pragma region Helpers

static void resizeThis(GUI::Button& button) {
   float buttonWidth = std::max(GetScreenWidth() / 2.0f, 400.0f);
   float buttonHeight = std::max(GetScreenHeight() / 20.0f, 40.0f);

   button.setBounds({ buttonWidth, buttonHeight }, true);
}

void HomeLayer::resize() {
   // Buttons
   resizeThis(m_newButton);
   if(m_newButton.getFontSize() != m_continueButton.getFontSize())
      LOG_RESIZE("Play button font -> {}", m_newButton.getFontSize());
   resizeThis(m_continueButton);

   Vector2 buttonOrigin = {
      (GetScreenWidth() - m_newButton.getSize().x) / 2.0f,
      GetScreenHeight() - PanelLayer::height - m_newButton.getSize().y * 3.0f };
   m_continueButton.setOrigin(buttonOrigin);
   buttonOrigin.y += m_newButton.getSize().y * 1.2f;
   m_newButton.setOrigin(buttonOrigin);

   // Background
   Vector2 old = { m_bgScale * m_backgroundTexture.width, m_bgScale * m_backgroundTexture.height };
   if(GetScreenWidth() > m_bgScale * m_backgroundTexture.width)
      m_bgScale = (float)GetScreenWidth() / m_backgroundTexture.width;
   else if(GetScreenHeight() > m_bgScale * m_backgroundTexture.height)
      m_bgScale = (float)GetScreenHeight() / m_backgroundTexture.height;

   if(old.x != m_bgScale * m_backgroundTexture.width || old.y != m_bgScale * m_backgroundTexture.height)
      LOG_RESIZE("Home background -> {} x {}", m_backgroundTexture.width, m_backgroundTexture.height);

   // Best Score's Trophy Icon
   float oldHeight = m_trophyScale * m_trophyTexture.height; 
   m_trophyScale = std::clamp(GetScreenHeight() / 800.0f, 1.0f, 2.5f);
   if(m_trophyScale * m_trophyTexture.height != oldHeight)
      LOG_RESIZE("Best score icon -> {} x {}", m_trophyTexture.width, m_trophyTexture.height);
}

GUI::Button* HomeLayer::findHoveredButton() {
   if(m_newButton.isHovered)
      return &m_newButton;
   else if(m_continueButton.isHovered)
      return &m_continueButton;
   else
      return nullptr;
}

#pragma endregion