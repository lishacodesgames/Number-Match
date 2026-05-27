#include <pch/Precompiled.h>
#include "GameLayer.h"

#include "Core/Logging.h"
#include "OptionsLayer.h"
#include "Core/Layer.h"
#include "PanelLayer.h"
#include "HomeLayer.h"
#include "Storage.h"
#include "App.h"

/// @bug matchability doesn't check if a cell is already matched
/// @todo make scoring system into an enum class (diagonal matches > non-adjacent matches > adjacent matches)

static constexpr Vector2 helperPadding = { 8, 8 };

float GridCell::size = 45.0f;
Color GridCell::hoverColor = ColorAlpha(SKYBLUE, 0.5f);
Color GridCell::focusedColor = ColorAlpha(BLUE, 0.5f);
Color GridCell::matchedColor = ColorAlpha(LIGHTERGRAY, 0.5f);

// clang-format off
GameLayer::GameLayer(bool reset) : Core::Layer("Game Layer"),
               m_grid(9, {0, 0, 0, 0, 0, 0, 0, 0, 0}),  // 9 rows of all blank cells
               m_focusedCell(nullptr),
               m_gobackButton({15, 15}, {0, 0}, "", BLANK, Color{42, 187, 235, 255}, 20, {0, 0}),
               m_settingsButton({0, 0}, {0, 0}, "", BLANK, Color{42, 187, 235, 255}, 20, {0, 0}),
               m_plusButton({0, 0}, helperPadding, "", LIGHTERGRAY, BLUE, 25, {1.0f, 8}),
               m_hintButton({0, 0}, helperPadding, "", LIGHTERGRAY, BLUE, 25, {1.0f, 8}) 
{  // clang-format on
   m_trophyTexture = LoadTexture("assets/icons/game/trophy_16x16.png");
   m_tickTexture = LoadTexture("assets/icons/game/tick_16x16.png");

   m_gobackButton.setIcon("assets/icons/game/goback_18x24.png");
   m_settingsButton.setIcon("assets/icons/game/settings_30x30.png");
   m_plusButton.setIcon("assets/icons/game/plus_35x35.png");
   m_hintButton.setIcon("assets/icons/game/hint_35x35.png");

   setButtonsOrigin();
   setGridBox();
   initGrid();

   if(reset)
      Storage::save(1, { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0);  // reset storage to default values
   else
      Storage::load();
}

void GameLayer::OnAttach() {
   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
   Core::Layer::OnAttach();
}

void GameLayer::OnResume() {
   setButtonsOrigin();
   setGridBox();
   setGridCells();
   Storage::load();

   Layer::OnResume();
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
         } else if(activeButton == &m_plusButton) {
            TraceLog(LISHA_SAYS, "PLUS");  // temp
         } else if(activeButton == &m_hintButton) {
            TraceLog(LISHA_SAYS, "HINT");  // temp
         }
         e.Handled = true;
         return;
      }

      // check if a grid cell has been clicked
      GridCell* activeCell = findHoveredGridCell();
      if(activeCell && activeCell != m_focusedCell) {  // new cell was clicked
         if(m_focusedCell) {                           // a cell already focused
            if(areCellsCompatible(getCellPos(m_focusedCell), getCellPos(activeCell))) {
               m_focusedCell->setState(CellState::Matched);
               activeCell->setState(CellState::Matched);
               m_focusedCell = nullptr;
            } else {  // if not compatible, set the new one as the focused cell
               m_focusedCell->setState(CellState::Rest);
               m_focusedCell = activeCell;
               if(*activeCell != 0)
                  activeCell->setState(CellState::Focused);
            }
         } else {  // no cell focused, so we set the clicked cell as the focused cell
            m_focusedCell = activeCell;
            if(*activeCell != 0)
               activeCell->setState(CellState::Focused);
         }
         e.Handled = true;
         return;
      } else if(!activeCell) {
         // no grid cell was clicked, so we reset the focused cell
         // since clicking outside of the grid or clicking a matched cell should deselect the cells
         // but we don't set e.Handled = true, in case the click has to be handled by another layer
         if(m_focusedCell) {
            m_focusedCell->setState(CellState::Rest);
            m_focusedCell = nullptr;
         }
      }
   }
}

void GameLayer::OnUpdate() {
   if(IsWindowResized()) {
      setButtonsOrigin();
      setGridBox();
      setGridCells();
   }

   if(isSuspended && !updateSuspended)
      return;

   m_gobackButton.Update();
   m_settingsButton.Update();
   m_plusButton.Update();
   m_hintButton.Update();

   static GridCell* previousCell = nullptr;  // to reset color back to restColor
   GridCell* hoveredCell = findHoveredGridCell();
   auto canOverride = [](GridCell* cell) -> bool {
      return cell && cell->getState() != CellState::Focused && cell->getState() != CellState::Matched;
   };  // helper lambda to check if we can override a cell's state (we can't override focused or matched cells)

   if(hoveredCell != previousCell) {  // a new cell is hovered
      if(canOverride(hoveredCell))
         hoveredCell->setState(CellState::Hovered);
      if(canOverride(previousCell))
         previousCell->setState(CellState::Rest);
   }
   previousCell = hoveredCell;

   if(hoveredCell || findHoveredButton())
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);

   Storage::hotReload();
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
   Color bgColor, numColor;
   for(size_t row = 0; row < m_grid.size(); row++) {
      for(size_t col = 0; col < m_grid.at(row).size(); col++) {
         GridCell cell = m_grid.at(row).at(col);
         num = cell.value ? std::to_string(cell.value) : "";  // empty string if value is 0 (empty cell)
         numSize = MeasureTextEx(App::font_semibold, num.c_str(), 40, 1);

         if(cell.getState() == CellState::Matched) {
            numColor = LIGHTGRAY;
            bgColor = GridCell::matchedColor;
         } else {
            numColor = BLACK;
            if(cell.getState() == CellState::Focused)
               bgColor = GridCell::focusedColor;
            else if(cell.getState() == CellState::Hovered)
               bgColor = GridCell::hoverColor;
            else
               bgColor = GridCell::restColor;
         }

         DrawRectangleRec(cell.cell, bgColor);
         DrawRectangleLinesEx(cell.cell, 1, ColorAlpha(LIGHTGRAY, 0.65f));
         DrawTextEx(
               App::font_semibold, num.c_str(),
               { cell.cell.x + cell.cell.width / 2 - numSize.x / 2, cell.cell.y + cell.cell.height / 2 - numSize.y / 2 },
               40, 1, numColor);
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

   // Stage
   DrawText("Stage", m_gridBox.x, tagY, tagFontSize, GRAY);
   DrawTextEx(App::font_semibold, std::to_string(Storage::stage).c_str(), { m_gridBox.x + 5, infoY }, infoFontSize + 3, infoFontSpacing, DARKGRAY);

   int scoreTagWidth = MeasureText("Best Score", tagFontSize);
   int scoreTagX = m_gridBox.x + m_gridBox.width - scoreTagWidth;
   int scoreValueWidth = MeasureTextEx(App::font_semibold, Storage::formatBestScore().c_str(), infoFontSize, infoFontSpacing).x;
   int scoreInfoX = m_gridBox.x + m_gridBox.width - scoreValueWidth - m_trophyTexture.width - 2;

   // Best Score
   DrawText("Best Score", scoreTagX, tagY, tagFontSize, GRAY);
   DrawTexture(m_trophyTexture, scoreInfoX, infoY, DARKGRAY);
   DrawTextEx(
         App::font_semibold, Storage::formatBestScore().c_str(),
         { (float)scoreInfoX + m_trophyTexture.width + 2, infoY },
         infoFontSize, infoFontSpacing, DARKGRAY);

   // Numbers Cleared
   int numbersTagWidth = MeasureText("Numbers Cleared", tagFontSize);
   int numbersTagX = m_gridBox.x + m_gridBox.width / 2 - numbersTagWidth / 2;
   // int numbersWidth = MeasureTextEx(App::font_semibold, "1 2 3 4 5 6 7 8 9", infoFontSize, infoFontSpacing).x;
   // int numbersX = m_gridBox.x + m_gridBox.width/2 - numbersWidth/2;

   DrawText("Numbers Cleared", numbersTagX, tagY, tagFontSize, GRAY);

   int numWidth;
   float numX;
   for(uint32_t i = 0; i < Storage::numbersCleared.size(); i++) {
      num = std::to_string(i + 1);
      numX = m_gridBox.x + m_gridBox.width / 2 - 60 + i * 15;

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
   int currentScoreWidth = MeasureTextEx(App::font_black, Storage::formatCurrentScore().c_str(), 40, 1).x;
   DrawTextEx(
         App::font_black, Storage::formatCurrentScore().c_str(),
         { (float)GetScreenWidth() / 2 - currentScoreWidth / 2, 58 },
         40, 1, DARKERGRAY);
}

void GameLayer::initGrid() {
   srand(time(0));

   for(size_t row = 0; row < m_grid.size(); row++) {
      for(size_t col = 0; col < m_grid.at(row).size(); col++) {
         if(row < 3 || (row == 3 && col < 5))
            m_grid[row][col].value = 1 + rand() % 9;
         else
            m_grid[row][col].value = 0;

         m_grid[row][col].setState(CellState::Rest);
      }
   }
   setGridCells();
}

#pragma region Helpers

void GameLayer::setButtonsOrigin() {
   m_settingsButton.setOrigin(GetScreenWidth() - 45, 15);

   int gameButtonsY = GetScreenHeight() - 60;
   m_plusButton.setOrigin(GetScreenWidth() / 2 - m_plusButton.getSize().x - 10, gameButtonsY);
   m_hintButton.setOrigin(GetScreenWidth() / 2 + 10, gameButtonsY);
}

void GameLayer::setGridBox() {
   GridCell::size = std::min(GetScreenWidth() * 0.6f / 9, 50.0f);  // since there are 9 cells in a row

   Vector2 boxOrigin = { ((float)GetScreenWidth() - GridCell::size * 9) / 2, 130 };
   m_gridBox = { boxOrigin.x, boxOrigin.y, GridCell::size * 9, GridCell::size * 9 };
}

void GameLayer::setGridCells() {
   for(size_t row = 0; row < m_grid.size(); row++) {
      for(size_t col = 0; col < m_grid.at(row).size(); col++) {
         m_grid[row][col].cell = {
            m_gridBox.x + col * GridCell::size, m_gridBox.y + row * GridCell::size,
            GridCell::size, GridCell::size
         };
      }
   }
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

GridCell* GameLayer::findHoveredGridCell() {
   for(auto& row : m_grid) {  // Not const because we want to return a non-const pointer
      for(GridCell& cell : row) {
         if(CheckCollisionPointRec(GetMousePosition(), cell.cell))
            return &cell;
      }
   }
   return nullptr;
}

std::pair<int, int> GameLayer::getCellPos(GridCell* cell) const {
   for(size_t row = 0; row < m_grid.size(); row++) {
      for(size_t col = 0; col < m_grid.at(row).size(); col++) {
         if(&m_grid[row][col] == cell)
            return { row, col };
      }
   }

   return { -1, -1 };  // default case (should not happen)
}

bool GameLayer::areCellsCompatible(std::pair<int, int> pos1, std::pair<int, int> pos2) const {
   GridCell cell1 = m_grid.at(pos1.first).at(pos1.second);
   GridCell cell2 = m_grid.at(pos2.first).at(pos2.second);

   if(cell1.getState() == CellState::Matched || cell2.getState() == CellState::Matched)
      return false;  // matched cells are not compatible with any cell

   // 1. Value Compatibility
   bool areValuesCompatible =  // cells sum to 10 or are equal but are not empty.
         (cell1 + cell2 == 10) ||
         (cell1 == cell2 && cell1 != 0);
   if(!areValuesCompatible)
      return false;

   // 2. Same row/column IF no unmatched cell in between
   if(pos1.first == pos2.first) {  // same row
      int row = pos1.first;
      int colStart = std::min(pos1.second, pos2.second) + 1;
      int colEnd = std::max(pos1.second, pos2.second);
      for(int col = colStart; col < colEnd; col++)
         if(m_grid.at(row).at(col).getState() != CellState::Matched)
            return false;  // if there is a non-matched cell in between, the cells are not compatible

      return true;
   } else if(pos1.second == pos2.second) {  // same column
      int col = pos1.second;
      int rowStart = std::min(pos1.first, pos2.first) + 1;
      int rowEnd = std::max(pos1.first, pos2.first);
      for(int row = rowStart; row < rowEnd; row++)
         if(m_grid.at(row).at(col).getState() != CellState::Matched)
            return false;  // if there is a non-matched cell in between, the cells are not compatible

      return true;
   }

   // 3. Same diagonal IF no unmatched cell in between
   int rowDiff = std::abs(pos2.first - pos1.first);
   int colDiff = std::abs(pos2.second - pos1.second);
   if(rowDiff == colDiff) {  // same diagonal
      int rowStep = pos1.first > pos2.first ? -1 : 1;
      int colStep = pos1.second > pos2.second ? -1 : 1;

      // adding step so we don't include the cell's row/col
      int colStart = pos1.second + colStep;
      int rowStart = pos1.first + rowStep;

      for(int row = rowStart, col = colStart; row != pos2.first; row += rowStep, col += colStep) {
         if(m_grid.at(row).at(col).getState() != CellState::Matched)
            return false;  // if there is a non-matched cell in between, the cells are not compatible
      }

      return true;
   }

   // 4. If all cells to the right of cell are matched, its "vision" wraps around to the first cell unmatched cell of next row
   if(rowDiff == 1) { // rows must be adjacent, since there cannot be a fully matched row
      if(pos1.first > pos2.first) {
         std::swap(pos1, pos2);  // we want pos1 to be the upper cell and pos2 to be the lower cell
         std::swap(cell1, cell2);
      }

      int colStart = pos1.second + 1;
      bool isRow1Clear = true;
      for(size_t col = colStart; col < m_grid.at(pos1.first).size(); col++) {
         if(m_grid.at(pos1.first).at(col).getState() != CellState::Matched) {
            isRow1Clear = false;
            break;
         }
      }

      if(isRow1Clear) {
         int colEnd = pos2.second;
         for(int col = 0; col < colEnd; col++) {
            if(m_grid.at(pos2.first).at(col).getState() != CellState::Matched)
               return false;
         }

         return true;
      }

   }

   return false;  // did not pass any compatibility condition, hence incompatible
}

#pragma endregion