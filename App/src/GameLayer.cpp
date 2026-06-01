#include <pch/Precompiled.h>
#include "GameLayer.h"

#include "Core/Logging.h"
#include "OptionsLayer.h"
#include "Core/Layer.h"
#include "PanelLayer.h"
#include "HomeLayer.h"
#include "Storage.h"
#include "Colors.h"
#include "App.h"

/// @todo make scoring system into an enum class (diagonal matches > non-adjacent matches > adjacent matches)

static constexpr Vector2 helperPadding = { 8, 8 };

GameLayer::GameLayer(bool reset) : Core::Layer("Game Layer"),
      m_gobackButton({15, 15}, {0, 0}, "", BLANK, Color{42, 187, 235, 255}, 20, {0, 0}),
      m_settingsButton({0, 0}, {0, 0}, "", BLANK, Color{42, 187, 235, 255}, 20, {0, 0}),
      m_plusButton({0, 0}, helperPadding, "", LIGHTERGRAY, BLUE, 25, {1.0f, 8}),
      m_hintButton({0, 0}, helperPadding, "", LIGHTERGRAY, BLUE, 25, {1.0f, 8}) 
{
   m_trophyTexture = LoadTexture("assets/icons/game/trophy_16x16.png");
   m_tickTexture = LoadTexture("assets/icons/game/tick_16x16.png");

   m_gobackButton.setIcon("assets/icons/game/goback_18x24.png");
   m_settingsButton.setIcon("assets/icons/game/settings_30x30.png");
   m_plusButton.setIcon("assets/icons/game/plus_35x35.png");
   m_hintButton.setIcon("assets/icons/game/hint_35x35.png");

   setButtonsOrigin();

   if(reset)
      Storage::save(1, { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0);  // reset storage to default values
   else
      Storage::load();
}

void GameLayer::OnEvent(Core::Event& e) {
   if(isSuspended && !eventSuspended)
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
         }
         else if(activeButton == &m_plusButton)
            m_grid.plus();
         else if(activeButton == &m_hintButton)
            TraceLog(LISHA_SAYS, "HINT");  // temp

         e.Handled = true;
         return;
      }

      // check if a grid cell has been clicked
      GridCell* activeCell = m_grid.findHoveredCell();
      if(activeCell) {
         if(activeCell != m_grid.focusedCell) {  // new cell was clicked
            if(m_grid.isCellCompatible(m_grid.getCellPos(activeCell)))
               handleMatch(activeCell);
            else {
               if(m_grid.focusedCell)
                  m_grid.focusedCell->setState(CellState::Rest);

               m_grid.focusedCell = activeCell;
               if(*activeCell != 0) // empty cells cannot be focused
                  activeCell->setState(CellState::Focused);
            }
         } else {  // clicking the already focused cell should deselect it
            activeCell->setState(CellState::Hovered);
            m_grid.focusedCell = nullptr;
         }
         e.Handled = true;
         return;
      } else if(m_grid.focusedCell) {
         // clicking a matched cell or outside of the grid or a matched cell should deselect the cells
         m_grid.focusedCell->setState(CellState::Rest);
         m_grid.focusedCell = nullptr;
         // we don't set e.Handled = true, in case the click has to be handled by another layer
      }
   }
}

void GameLayer::OnUpdate() {
   if(IsWindowResized()) {
      setButtonsOrigin();
      m_grid.resize();
   }

   if(isSuspended && !updateSuspended)
      return;

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
   if(isSuspended && !renderSuspended)
      return;

   m_gobackButton.Draw();
   m_settingsButton.Draw();
   m_plusButton.Draw();
   m_hintButton.Draw();

   m_grid.Draw();

   // Game info
   int tagFontSize = 5;
   float tagY = m_grid.box.y - 30;

   int infoFontSize = 17;
   float infoFontSpacing = 0.98f;
   float infoY = m_grid.box.y - 20;

   // Stage
   DrawText("Stage", m_grid.box.x, tagY, tagFontSize, GRAY);
   DrawTextEx(App::font_semibold, std::to_string(Storage::stage).c_str(), { m_grid.box.x + 5, infoY }, infoFontSize + 3, infoFontSpacing, DARKGRAY);

   int scoreTagWidth = MeasureText("Best Score", tagFontSize);
   int scoreTagX = m_grid.box.x + m_grid.box.width - scoreTagWidth;
   int scoreValueWidth = MeasureTextEx(App::font_semibold, Storage::formatBestScore().c_str(), infoFontSize, infoFontSpacing).x;
   int scoreInfoX = m_grid.box.x + m_grid.box.width - scoreValueWidth - m_trophyTexture.width - 2;

   // Best Score
   DrawText("Best Score", scoreTagX, tagY, tagFontSize, GRAY);
   DrawTexture(m_trophyTexture, scoreInfoX, infoY, DARKGRAY);
   DrawTextEx(
         App::font_semibold, Storage::formatBestScore().c_str(),
         { (float)scoreInfoX + m_trophyTexture.width + 2, infoY },
         infoFontSize, infoFontSpacing, DARKGRAY);

   // Numbers Cleared
   int numbersTagWidth = MeasureText("Numbers Cleared", tagFontSize);
   int numbersTagX = m_grid.box.x + m_grid.box.width / 2 - numbersTagWidth / 2;

   DrawText("Numbers Cleared", numbersTagX, tagY, tagFontSize, GRAY);

   for(uint32_t i = 0; i < Storage::numbersCleared.size(); i++) {
      std::string num = std::to_string(i + 1);
      float numX = m_grid.box.x + m_grid.box.width / 2 - 60 + i * 15;

      int numWidth;
      if(Storage::numbersCleared.at(i)) {
         numWidth = m_tickTexture.width;  // if number is cleared, we draw a tick mark at its position
         numX -= numWidth / 2;            // center the tick mark at the number's position
         DrawTexture(m_tickTexture, numX, infoY, GRAY);
      } else {
         numWidth = MeasureTextEx(App::font_semibold, num.c_str(), infoFontSize, infoFontSpacing).x;
         numX -= numWidth / 2;  // center the number in its position
         DrawTextEx(App::font_semibold, num.c_str(), { numX, infoY }, infoFontSize, infoFontSpacing, DARKGRAY);
      }
   }

   // Current Score
   int currentScoreWidth = MeasureTextEx(App::font_black, Storage::formatCurrentScore().c_str(), GridCell::numHeight, 1).x;
   DrawTextEx(
      App::font_black, Storage::formatCurrentScore().c_str(),
      { (float)GetScreenWidth() / 2 - currentScoreWidth / 2, 58 },
      GridCell::numHeight, 1, DARKERGRAY
   );
}

void GameLayer::OnResume() {
   setButtonsOrigin();
   m_grid.resize();
   Storage::load();

   Layer::OnResume();
}

void GameLayer::handleMatch(GridCell* cell) {
   m_grid.focusedCell->setState(CellState::Matched);
   cell->setState(CellState::Matched);

   // check if either cell's row is clear
   int row1 = m_grid.getCellPos(m_grid.focusedCell).first;
   int row2 = m_grid.getCellPos(cell).first;

   if(m_grid.isRowClear(row1))
      m_grid.clearRow(row1);
   
   if(row2 > row1)
      row2--;
   
   if(m_grid.isRowClear(row2))
      m_grid.clearRow(row2);

   m_grid.focusedCell = nullptr;
}

#pragma region Helpers

void GameLayer::setButtonsOrigin() {
   m_settingsButton.setOrigin(GetScreenWidth() - 45, 15);

   int gameButtonsY = GetScreenHeight() - 60;
   m_plusButton.setOrigin(GetScreenWidth() / 2 - m_plusButton.getSize().x - 10, gameButtonsY);
   m_hintButton.setOrigin(GetScreenWidth() / 2 + 10, gameButtonsY);
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