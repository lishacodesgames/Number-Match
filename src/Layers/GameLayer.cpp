#include <pch/Precompiled.h>
#include "GameLayer.h"

#include "OptionsLayer.h"
#include "PanelLayer.h"
#include "HomeLayer.h"
#include "CoinLayer.h"
#include "Storage.h"
#include "Colors.h"
#include "App.h"

GameLayer::GameLayer(bool reset) : Core::Layer("Game Layer"), m_grid(reset),
      m_trophyTexture(LoadTexture("assets/icons/game/trophy_16x16.png")),
      m_tickTexture(LoadTexture("assets/icons/game/tick_16x16.png")),
      m_gobackButton({ 15, 15 }, { 6, 1 }, { 1, 1 }, "", BLANK, Palette::game_nav_color, 20, { 0, 0 }),
      m_settingsButton({ 0, 0 }, { 1, 1 }, "", BLANK, Palette::game_nav_color, 20, { 0, 0 }),
      m_plusButton(
         { 0, 0 }, { 12, 10 }, "", Palette::game_button_bg, Palette::game_button_text, 25, { 1.0f, 8 }),
      m_hintButton(
         { 0, 0 }, { 12, 10 }, "", Palette::game_button_bg, Palette::game_button_text, 25, { 1.0f, 8 }) 
{
   TraceLog(LISHA_SAYS, "Loading a new game...");

   m_gobackButton.setIcon("assets/icons/game/goback_18x24.png");
   m_settingsButton.setIcon("assets/icons/game/settings_30x30.png");
   m_plusButton.setIcon("assets/icons/game/plus_35x35.png");
   m_hintButton.setIcon("assets/icons/game/hint_35x35.png");

   resize();

   if(reset) {
      Storage::game.stage = 1;
      Storage::game.currentScore = 0;
   } // numbersCleared initalisation is handled by Grid constructor

   TraceLog(LISHA_SAYS, "New game loaded!");
}

GameLayer::~GameLayer() {
   UnloadTexture(m_trophyTexture);
   UnloadTexture(m_tickTexture);
   Storage::save(m_grid.getSaveData());
}

#pragma region Methods

void GameLayer::OnEvent(Core::Event& e) {
   if(e.GetEventType() == Core::EventType::KeyPressed) {
      char key = static_cast<Core::KeyPressedEvent&>(e).key;
      if(key == 'q' || key == 'Q') {
         OnSuspend();
         App::QueueLayerPush(new HomeLayer());
         App::QueueLayerPush(new PanelLayer());
         e.Handled = true;
         return;
      }
   } else if(e.GetEventType() == Core::EventType::MouseClicked) {
      // check if a button has been clicked
      GUI::Button* activeButton = findHoveredButton();
      if(activeButton) {
         if(activeButton == &m_gobackButton) {
            OnSuspend();
            App::QueueLayerPush(new HomeLayer());
            App::QueueLayerPush(new PanelLayer());
         } else if(activeButton == &m_settingsButton) {
            OnSuspend(true);  // suspend but render
            App::QueueLayerPush(new OptionsLayer());
         } else if(activeButton == &m_plusButton) {
            if(m_plusButton.bgColor == Palette::grid_hint)
               m_plusButton.bgColor = Palette::game_button_bg;
            m_grid.plus();
         } else if(activeButton == &m_hintButton)
            if(!m_grid.hint())
               m_plusButton.bgColor = Palette::grid_hint;

         e.Handled = true;
      }

      // check and handle if a grid cell has been clicked
      e.Handled = m_grid.OnClick() || e.Handled;
   }
}

void GameLayer::OnUpdate() {
   if(IsWindowResized())
      resize();

   m_gobackButton.Update();
   m_settingsButton.Update();
   m_plusButton.Update();
   m_hintButton.Update();

   m_grid.Update();

   if(findHoveredButton() || m_grid.findHoveredCell())
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void GameLayer::OnRender() {
   m_gobackButton.Draw();
   m_settingsButton.Draw();

   // if theme is changed, we must update the colors
   /// @todo I don't like this. Pls fix
   static bool previous = Storage::ui.isDarkMode;
   if(previous != Storage::ui.isDarkMode) {
      m_plusButton.bgColor = Palette::game_button_bg;
      m_plusButton.contentColor = Palette::game_button_text;
      m_hintButton.bgColor = Palette::game_button_bg;
      m_hintButton.contentColor = Palette::game_button_text;
      previous = Storage::ui.isDarkMode;
   }

   m_plusButton.Draw();
   m_hintButton.Draw();

   m_grid.Draw();

   // Game info
   Rectangle gameInfoBox = { m_grid.box.x, m_grid.box.y - GridCell::cellSize * 0.7f, m_grid.box.width, GridCell::cellSize * 0.7f };
   float spacing = 0.98f;

   float tagY = gameInfoBox.y - gameInfoBox.height * 0.10f;
   float tagFontSize = gameInfoBox.height * 0.55f;

   float infoFontSize = tagFontSize * 0.94f;
   float infoY = m_grid.box.y - infoFontSize * 1.1f;

   // Stage
   DrawTextEx(
      App::font_retro, "Stage", { m_grid.box.x, tagY },
      tagFontSize, spacing, Palette::game_info_color);
   DrawTextEx(
      App::font_semibold, Storage::format(Storage::game.stage).c_str(), { m_grid.box.x + 5, infoY },
      infoFontSize + 3, spacing, Palette::game_info_color);

   // Best Score
   float scoreTagX =
      m_grid.box.x + m_grid.box.width
         - MeasureTextEx(App::font_retro, "Best Score", tagFontSize, spacing).x;
   float scoreInfoX =
      m_grid.box.x + m_grid.box.width
         - MeasureTextEx(App::font_semibold, Storage::format(Storage::game.bestScore).c_str(), infoFontSize, spacing).x
         - m_trophyScale * m_trophyTexture.width * 1.0005f;

   DrawTextEx(App::font_retro, "Best Score", { scoreTagX, tagY }, tagFontSize, spacing, Palette::game_info_color);
   DrawTextEx(
      App::font_semibold, Storage::format(Storage::game.bestScore).c_str(),
      { scoreInfoX + m_trophyScale * m_trophyTexture.width * 1.0005f, infoY },
      infoFontSize, spacing, Palette::game_info_color);

   if(Storage::game.bestScore == Storage::game.currentScore) // only draw if current is the best 
      DrawTextureEx(m_trophyTexture, { scoreInfoX, infoY }, 0.0f, m_trophyScale, Palette::game_info_color);

   // Numbers Cleared
   float numbersTagX =
      m_grid.box.x + m_grid.box.width / 2.0f
         - MeasureTextEx(App::font_retro, "Numbers Cleared", tagFontSize, spacing).x / 2.0f;
   DrawTextEx(
      App::font_retro, "Numbers Cleared", { numbersTagX, tagY },
      tagFontSize, spacing, Palette::game_info_color);

   float numAndPadWidth = m_tickScale * m_tickTexture.width * 1.15f; // 50% width for padding per num
   float allNumsWidth = numAndPadWidth * 9 - m_tickScale * m_tickTexture.width * 0.5f; // 8 pads for 9 nums
   float numX = m_grid.box.x + (m_grid.box.width - allNumsWidth) / 2;

   for(int i = 0; i < 9; i++) {
      std::string num = std::to_string(i + 1);

      if(Storage::game.numbersCleared.at(i))
         DrawTextureEx(m_tickTexture, { numX, infoY }, 0.0f, m_tickScale, Palette::game_info_color);
      else {
         Vector2 numSize = MeasureTextEx(App::font_semibold, num.c_str(), infoFontSize, spacing);
         DrawTextEx(
            App::font_semibold, num.c_str(),
            {  numX + (m_tickScale * m_tickTexture.width - numSize.x) / 2.0f,
               infoY + (m_tickScale * m_tickTexture.height - numSize.y) / 2.0f },
            infoFontSize, spacing, Palette::game_info_color);
      }

      numX += numAndPadWidth;
   }

   // Current Score
   float remSpaceY = tagY - (CoinLayer::box.y + CoinLayer::box.height);
   float currentScoreFontSize = std::min(GridCell::numHeight, remSpaceY * 0.8f);
   Vector2 currentScoreSize = MeasureTextEx(App::font_black, Storage::format(Storage::game.currentScore).c_str(), currentScoreFontSize, 1);
   DrawTextEx(
      App::font_black, Storage::format(Storage::game.currentScore).c_str(),
      { (float)GetScreenWidth() / 2 - currentScoreSize.x / 2, tagY - currentScoreSize.y * 1.2f },
      currentScoreFontSize, 1, Palette::text_for_off_bright);
}

#pragma endregion

#pragma region Helpers

void GameLayer::resize() {
   // game
   m_grid.resize();

   float infoFontSize = GridCell::numHeight * 0.55f * 0.9f;
   m_trophyScale = std::max(infoFontSize / m_trophyTexture.height, 1.0f);
   if(m_trophyScale > 1.25f || m_trophyScale < 0.75f)
      LOG_RESIZE("Best Score Trophy resized to: %d, %d", m_trophyTexture.width, m_trophyTexture.height);

   m_tickScale = std::max(infoFontSize / m_tickTexture.height, 1.0f);
   if(m_tickScale > 1.25f || m_tickScale < 0.75f)
      LOG_RESIZE("Tick resized to: %d, %d", m_tickTexture.width, m_tickTexture.height);

   // navigation buttons
   m_gobackButton.setFontSize(GridCell::numHeight);
   m_settingsButton.setFontSize(GridCell::numHeight);
   
   m_gobackButton.setOrigin({ m_gobackButton.getSize().x * 0.3f, m_gobackButton.getSize().y * 0.3f });
   m_settingsButton.setOrigin({ GetScreenWidth() - m_settingsButton.getSize().x * 1.3f, m_settingsButton.getSize().y * 0.3f });

   // gameplay buttons
   float remSpaceY = GetScreenHeight() - (m_grid.box.y + m_grid.box.height); // remaining space below grid
   float iconHeight = std::min(GridCell::numHeight, remSpaceY * 0.5f);
   float iconWidth = iconHeight; // for padding readability
   m_plusButton.setFontSize(iconHeight);
   m_hintButton.setFontSize(iconHeight);
   m_plusButton.setPadding(iconWidth * 0.6f, iconHeight * 0.3f);
   m_hintButton.setPadding(iconWidth * 0.6f, iconHeight * 0.3f);

   int gameButtonsY = m_grid.box.y + m_grid.box.height + (remSpaceY - m_plusButton.getSize().y) / 2;
   m_plusButton.setOrigin(GetScreenWidth() / 2 - m_plusButton.getSize().x * 1.3f, gameButtonsY);
   m_hintButton.setOrigin(GetScreenWidth() / 2 + m_plusButton.getSize().x * 0.3f, gameButtonsY);
}

GUI::Button* GameLayer::findHoveredButton() {
   if(m_gobackButton.isHovered)
      return &m_gobackButton;
   else if(m_settingsButton.isHovered)
      return &m_settingsButton;
   else if(m_plusButton.isHovered)
      return &m_plusButton;
   else if(m_hintButton.isHovered)
      return &m_hintButton;
   else
      return nullptr;
}

#pragma endregion
