#include <pch/Precompiled.h>
#include "GameLayer.h"

#include "Core/Logging.h"
#include "OptionsLayer.h"
#include "Core/Layer.h"
#include "PanelLayer.h"
#include "HomeLayer.h"
#include "Storage.h"
#include "App.h"

static constexpr Vector2 helperPadding = {8, 8};

Color GridCell::focusedColor = ColorAlpha(BLUE, 0.5f);
Color GridCell::hoverColor = ColorAlpha(SKYBLUE, 0.5f);

GameLayer::GameLayer() : Core::Layer("Game Layer"),
      m_gridBox({(float)(GetScreenWidth())/2 - 202.5f, 130, 405, 405}),
      m_grid(9, {0, 0, 0, 0, 0, 0, 0, 0, 0}), // 9 rows of all blank cells
      m_focusedCells{nullptr, nullptr},
      m_gobackButton({15, 15}, {0, 0}, "", BLANK, Color{42, 187, 235, 255}, 20, {0, 0}),
      m_settingsButton(
         {static_cast<float>(GetScreenWidth()) - 45, 15}, {0, 0}, "", BLANK, Color{42, 187, 235, 255}, 20, {0, 0}
      ),
      m_plusButton(
         {static_cast<float>(GetScreenWidth()) / 2 - 49, static_cast<float>(GetScreenHeight()) - 60}, // origin
         helperPadding, "", LIGHTERGRAY, BLUE, 25, {1.0f, 8}
      ),
      m_hintButton(
         {static_cast<float>(GetScreenWidth()) / 2 + 14, static_cast<float>(GetScreenHeight()) - 60}, // origin
         helperPadding, "", LIGHTERGRAY, BLUE, 25, {1.0f, 8}
      )
{      
   initGrid();

   m_trophyTexture = LoadTexture("assets/icons/game/trophy_16x16.png");
   m_tickTexture = LoadTexture("assets/icons/game/tick_16x16.png");

   m_gobackButton.setIcon("assets/icons/game/goback_18x24.png");
   m_settingsButton.setIcon("assets/icons/game/settings_30x30.png");
   m_plusButton.setIcon("assets/icons/game/plus_35x35.png");
   m_hintButton.setIcon("assets/icons/game/hint_35x35.png");
}

void GameLayer::OnAttach() {
   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
   Core::Layer::OnAttach();   
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
            OnSuspend(true); // suspend but render
            App::QueueLayerPush(new OptionsLayer());
         } else if(activeButton == &m_plusButton) {
            TraceLog(LISHA_SAYS, "PLUS"); // temp
         } else if(activeButton == &m_hintButton) {
            TraceLog(LISHA_SAYS, "HINT"); // temp
         }
         e.Handled = true;
         return;
      }

      // check if a grid cell has been clicked
      GridCell* activeCell = findHoveredGridCell();
      if(activeCell) { // a cell was clicked
         if(!isFocused(activeCell)) { // new cell was clicked
            if(m_focusedCells.second) { // 2 cells already focused, reset both and set the new one as the only focused cell
               m_focusedCells.first->state = GridCellState::Rest;
               m_focusedCells.second->state = GridCellState::Rest;
               m_focusedCells = {activeCell, nullptr};
            } else { // either 1 or 0 focused cells
               bool firstFocusedCellIsCompatible = false; 
               if(m_focusedCells.first) {
                  firstFocusedCellIsCompatible = (
                     *m_focusedCells.first + *activeCell == 10 || 
                     (m_focusedCells.first->value == activeCell->value && activeCell->value != 0)
                  ); // cells sum to 10 or are equal but are not empty.
               }

               if(firstFocusedCellIsCompatible)
                  m_focusedCells.second = activeCell;
               else { // reset the first focused cell and set the new one as the only focused cell
                  if(m_focusedCells.first)
                     m_focusedCells.first->state = GridCellState::Rest;
                  m_focusedCells = {activeCell, nullptr};
               }
            }
            
            activeCell->state = GridCellState::Focused;
         } // no change if a focused cell was clicked
         
         e.Handled = true;
         return;
      }
      
      // no grid cell was clicked, so we reset the focused cells to none, since clicking outside of the grid deselects the cells
      // but we don't set e.Handled to true, in case the click has to be handled by another layer
      if(m_focusedCells.first) {
         m_focusedCells.first->state = GridCellState::Rest;
         m_focusedCells.first = nullptr;

         if(m_focusedCells.second) { // second can't exist without first. If it does, this nesting helps weed out bugs
            m_focusedCells.second->state = GridCellState::Rest;
            m_focusedCells.second = nullptr;
         }
      }
   }
}

void GameLayer::OnUpdate() {
   if(isSuspended && !updateSuspended)
      return;
   
   m_gobackButton.Update();
   m_settingsButton.Update();
   m_plusButton.Update();
   m_hintButton.Update();

   static GridCell* previousCell = nullptr; // to reset color back to restColor
   GridCell* hoveredCell = findHoveredGridCell();

   if(hoveredCell != previousCell) { // a new cell is hovered
      if(hoveredCell && !isFocused(hoveredCell)) // don't override focused cells
         hoveredCell->state = GridCellState::Hovered;

      if(previousCell && !isFocused(previousCell)) // reset previous cell, unless it's focused
         previousCell->state = GridCellState::Rest;
   }
   previousCell = hoveredCell;

   if(hoveredCell || findHoveredButton())
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);

   Storage::load();
}

void GameLayer::OnRender() {
   if(isSuspended && !renderSuspended)
      return;

   m_gobackButton.Draw();
   m_settingsButton.Draw();
   m_plusButton.Draw();
   m_hintButton.Draw();

   // Grid
   std::string num;
   Vector2 numSize;
   Color bgColor;
   for(size_t row = 0; row < m_grid.size(); row++) {
      for(size_t col = 0; col < m_grid.at(row).size(); col++) {
         GridCell cell = m_grid.at(row).at(col);
         num = cell.value ? std::to_string(cell.value) : ""; // empty string if value is 0 (empty cell)
         numSize = MeasureTextEx(App::font_semibold, num.c_str(), 40, 1);

         if(cell.state == GridCellState::Focused)
            bgColor = GridCell::focusedColor;
         else if(cell.state == GridCellState::Hovered)
            bgColor = GridCell::hoverColor;
         else
            bgColor = GridCell::restColor;

         DrawRectangleRec(cell.cell, bgColor);
         DrawRectangleLinesEx(cell.cell, 1, ColorAlpha(LIGHTGRAY, 0.65f));
         DrawTextEx(
            App::font_semibold, num.c_str(),
            {cell.cell.x + cell.cell.width/2 - numSize.x/2, cell.cell.y + cell.cell.height/2 - numSize.y/2},
            40, 1, BLACK
         );
      }
   }

   // Box
   DrawRectangleLinesEx(m_gridBox, 3, ColorAlpha(DARKGRAY, 0.8f));

   // Game info
   int tagFontSize = 5;
   float tagY = m_gridBox.y - 30;

   int infoFontSize = 17;
   float infoFontSpacing = 0.98f;
   float infoY = m_gridBox.y - 20; 

   DrawText("Stage", m_gridBox.x, tagY, tagFontSize, GRAY);
   DrawTextEx(App::font_semibold, std::to_string(Storage::stage).c_str(), {m_gridBox.x + 5, infoY}, infoFontSize + 3, infoFontSpacing, DARKGRAY);

   int scoreTagWidth = MeasureText("Best Score", tagFontSize);
   int scoreTagX = m_gridBox.x + m_gridBox.width - scoreTagWidth;
   int scoreValueWidth = MeasureTextEx(App::font_semibold, Storage::formatBestScore().c_str(), infoFontSize, infoFontSpacing).x;
   int scoreInfoX = m_gridBox.x + m_gridBox.width - scoreValueWidth - m_trophyTexture.width - 2;

   DrawText("Best Score", scoreTagX, tagY, tagFontSize, GRAY);
   DrawTexture(m_trophyTexture, scoreInfoX, infoY, DARKGRAY);
   DrawTextEx(
      App::font_semibold, Storage::formatBestScore().c_str(),
      {(float)scoreInfoX + m_trophyTexture.width + 2, infoY},
      infoFontSize, infoFontSpacing, DARKGRAY
   );

   int numbersTagWidth = MeasureText("Numbers Cleared", tagFontSize);
   int numbersTagX = m_gridBox.x + m_gridBox.width/2 - numbersTagWidth/2;
   // int numbersWidth = MeasureTextEx(App::font_semibold, "1 2 3 4 5 6 7 8 9", infoFontSize, infoFontSpacing).x;
   // int numbersX = m_gridBox.x + m_gridBox.width/2 - numbersWidth/2;

   DrawText("Numbers Cleared", numbersTagX, tagY, tagFontSize, GRAY);

   int numWidth;
   float numX;
   for(uint32_t i = 0; i < Storage::numbersCleared.size(); i++) {
      num = std::to_string(i + 1);
      numX = m_gridBox.x + m_gridBox.width/2 - 60 + i * 15;

      if(Storage::numbersCleared.at(i)) {
         numWidth = m_tickTexture.width; // if number is cleared, we draw a tick mark at its position
         numX -= numWidth/2; // center the tick mark at the number's position
         DrawTexture(m_tickTexture, numX, infoY, GRAY);
      } else {
         numWidth = MeasureTextEx(App::font_semibold, num.c_str(), infoFontSize, infoFontSpacing).x;
         numX -= numWidth/2; // center the number in its position
         DrawTextEx(App::font_semibold, num.c_str(), {numX, infoY}, infoFontSize, infoFontSpacing, DARKGRAY);
      }
   }
}

void GameLayer::initGrid() {
   srand(time(0));

   for(size_t row = 0; row < m_grid.size(); row++) {
      for(size_t col = 0; col < m_grid.at(row).size(); col++) {
         if(row < 3 || (row == 3 && col < 5))
            m_grid[row][col].value = 1 + rand() % 9;
         else
            m_grid[row][col].value = 0;

         GridCell& cell = m_grid[row][col];
         cell.cell = {m_gridBox.x + col * GridCell::size, m_gridBox.y + row * GridCell::size, GridCell::size, GridCell::size};
         cell.state = GridCellState::Rest;
      }
   }
}

#pragma region Helpers

GUI::Button* GameLayer::findHoveredButton() {
   if (m_gobackButton.isHovered)
      return &m_gobackButton;
   else if (m_settingsButton.isHovered)
      return &m_settingsButton;
   else if (m_plusButton.isHovered)
      return &m_plusButton;
   else if (m_hintButton.isHovered)
      return &m_hintButton;
   else
      return nullptr;
}

GridCell* GameLayer::findHoveredGridCell() {
   for(auto& row : m_grid) { // Not const because we want to return a non-const pointer
      for(GridCell& cell : row) {
         if(CheckCollisionPointRec(GetMousePosition(), cell.cell))
            return &cell;
      }
   }
   return nullptr;
}

#pragma endregion