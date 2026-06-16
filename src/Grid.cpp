#include <pch/Precompiled.h>
#include "Grid.h"

#include "Layers/CoinLayer.h"
#include "Storage.h"
#include "Colors.h"
#include "App.h"

float GridCell::cellSize = 45.0f;
float GridCell::numHeight = 0;

Grid::Grid() : m_focusedCell(nullptr),
      m_scrollBar({ 0, 0, 0, 0 }, 10.0f, Palette::grid_scroll_thumb, Palette::grid_scroll_track)
{
   m_grid.assign(9, { 0, 0, 0, 0, 0, 0, 0, 0, 0 }),  // 9 rows of all blank cells
   srand(time(0));

   init();
   resize();
   m_scrollBar.setTrackBounds({ box.x + box.width + 25, box.y, 2, box.height });
}

#pragma region Methods

bool Grid::OnClick() {
   GridCell* activeCell = findHoveredCell();
   if(activeCell) {
      if(activeCell != m_focusedCell) {  // new cell was clicked
         if(isCellCompatible(activeCell)) {
            handleMatch(activeCell);
         } else {
            if(m_focusedCell)
               m_focusedCell->setState(CellState::Rest);

            if(*activeCell != 0) { // empty cells cannot be focused
               m_focusedCell = activeCell;
               activeCell->setState(CellState::Focused);
            }
         }
      } else {  // clicking the already focused cell should deselect it
         activeCell->setState(CellState::Hovered);
         m_focusedCell = nullptr;
      }
      return true;
   } else if(m_focusedCell) {
      // clicking a matched cell or outside of the grid or a matched cell should deselect the cells
      m_focusedCell->setState(CellState::Rest);
      m_focusedCell = nullptr;
   }

   return false; // in case the click has to be handled by another layer
}

bool canOverride(GridCell* cell) {
   return cell && cell->getState() != CellState::Focused && cell->getState() != CellState::Matched;
} // we can't override focused or matched cells)   

void Grid::Update() {
   // Update hovered cell's state
   static GridCell* previousCell = nullptr;  // to reset color back to restColor
   GridCell* hoveredCell = findHoveredCell();

   if(hoveredCell != previousCell) {  // a new cell is hovered
      if(canOverride(hoveredCell))
         hoveredCell->setState(CellState::Hovered);
      if(canOverride(previousCell))
         previousCell->setState(CellState::Rest);
   }
   previousCell = hoveredCell;

   // Grid scroll logic
   /// @todo let user choose direction of mouse scroll (+ <-> -) in settings
   float maxScrollOffset = std::max(0.0f, m_grid.size() * GridCell::cellSize - box.height);
   m_scrollOffset = std::clamp(m_scrollOffset - 35 * GetMouseWheelMove(), 0.0f, maxScrollOffset);
   float topRowY = box.y - m_scrollOffset;

   for(size_t row = 0; row < m_grid.size(); row++)
      for(size_t col = 0; col < m_grid.at(row).size(); col++)
         m_grid[row][col].bounds.y = topRowY + row * GridCell::cellSize;  // only y changes with scroll

   m_scrollBar.Update(m_scrollOffset, m_grid.size() * GridCell::cellSize, box.height);
}

void Grid::Draw() const {
   Color bgColor, numColor;

   BeginScissorMode(box.x, box.y, box.width, box.height); // so grid cells don't render outside of the box
   for(size_t row = 0; row < m_grid.size(); row++) {
      for(size_t col = 0; col < m_grid.at(row).size(); col++) {
         const GridCell cell = m_grid.at(row).at(col);
         const std::string value = cell.value ? std::to_string(cell.value) : "";  // empty string if value is 0 (empty cell)
         const Vector2 numSize = MeasureTextEx(App::font_semibold, value.c_str(), GridCell::numHeight, 1);

         // state cases
         switch(cell.getState()) {
            case CellState::Rest:
               numColor = Palette::gridnum_not_matched;
               bgColor = Palette::off_bright_bg;
               break;
            case CellState::Hovered:
               numColor = Palette::gridnum_not_matched;
               bgColor = Palette::gridcell_hover;
               break;
            case CellState::Focused:
               numColor = Palette::gridnum_not_matched;
               bgColor = Palette::gridcell_focus;
               break;
            case CellState::Matched:
               numColor = Palette::gridnum_matched;
               bgColor = Palette::off_bright_bg;
               break;
         }

         DrawRectangleRec(cell.bounds, bgColor);
         DrawRectangleLinesEx(cell.bounds, 1, Palette::shadow_for_off_bright);
         DrawTextEx(
            App::font_semibold, value.c_str(),
            {  cell.bounds.x + (cell.bounds.width - numSize.x) / 2, 
               cell.bounds.y + (cell.bounds.height - numSize.y) / 2 },
            GridCell::numHeight, 1, numColor
         );
      }
   }
   EndScissorMode();

   DrawRectangleLinesEx(box, 3, Palette::gridbox_color);
   if(m_grid.size() > 9)
      m_scrollBar.Draw();
}

#pragma endregion

#pragma region Gameplay

void Grid::resize() {
   float old = GridCell::cellSize;
   GridCell::cellSize = // scaled based on smaller of the 2 screen dimensions
         std::max(std::min(GetScreenWidth(), GetScreenHeight()) * 0.7f / 9, 40.0f);
   GridCell::numHeight = GridCell::cellSize * 0.75f;

   if(std::abs(old - GridCell::cellSize) > 0.5f)
      LOG_RESIZE("Grid resized: cellSize = %f", GridCell::cellSize);

   box.width = box.height = GridCell::cellSize * 9;
   box.x = (GetScreenWidth() - box.width) / 2.0f;
   // some extra padding for the game info on top
   box.y = (GetScreenHeight() - box.height + GridCell::cellSize * 1.35f) / 2.0f;

   for(size_t row = 0; row < m_grid.size(); row++)
      for(size_t col = 0; col < m_grid.at(row).size(); col++) 
         setCellBounds(&m_grid[row][col]);

   m_scrollBar.setTrackBounds({ box.x + box.width + 25, box.y, 2, box.height });
}

void Grid::plus() {
   std::vector<int> plusValues;
   for(size_t row = 0; row < m_grid.size(); row++)
      for(size_t col = 0; col < m_grid.at(row).size(); col++)
         if(m_grid.at(row).at(col) != 0) // operator also checks for state != matched
            plusValues.push_back(m_grid.at(row).at(col).value);

   if(plusValues.empty())
      return; /// @todo stage system

   std::pair<int, int> firstEmptyCell = { -1, -1 };
   for(size_t row = 1; row < m_grid.size(); row++) { // first row can never be empty so we start from 2nd
      auto it = std::find(m_grid.at(row).begin(), m_grid.at(row).end(), 0);
      if(it != m_grid.at(row).end()) {
         firstEmptyCell = { row, static_cast<int>(std::distance(m_grid.at(row).begin(), it)) };
         break;
      }

      if(row == m_grid.size() - 1) { // if no empty cell till last iteration, we make a new row of empty cell
         m_grid.push_back({ 0, 0, 0, 0, 0, 0, 0, 0, 0 });
         firstEmptyCell = { m_grid.size() - 1, 0 };
      }
   }

   if(firstEmptyCell.first == -1) {
      TraceLog(LOG_ERROR, "Error in Grid::plus(): no empty cell found and failed to create new row");
      return;
   }

   // Duplicate all valid cells starting from firstEmptyCell
   int row = firstEmptyCell.first;
   int col = firstEmptyCell.second;
   for(int value : plusValues) {
      m_grid[row][col].value = value;

      // increment grid index
      if(++col == (int)m_grid.at(0).size()) {
         col = 0;
         row++;
         if(row == (int)m_grid.size()) {
            m_grid.push_back({ 0, 0, 0, 0, 0, 0, 0, 0, 0 });
            for(GridCell& cell : m_grid[row])
               setCellBounds(&cell);
         }
      } // if condition already increments col
   }
}

void Grid::handleMatch(GridCell* cell) {
   m_focusedCell->setState(CellState::Matched);
   cell->setState(CellState::Matched);
   
   int num1 = m_focusedCell->value;
   int num2 = cell->value;
   
   // check if either cell's number is clear
   if(isNumClear(num1))
      Storage::numbersCleared[num1 - 1] = true;
   if(num1 != num2 && isNumClear(num2))
      Storage::numbersCleared[num2 - 1] = true;

   // check if either cell's row is clear
   int row1 = getCellPos(m_focusedCell).first;
   int row2 = getCellPos(cell).first;

   if(isRowClear(row1)) {
      clearRow(row1);

      if(row2 > row1)
         row2--;
   }
   
   if(isRowClear(row2))
      clearRow(row2);

   m_focusedCell = nullptr;
}

#pragma endregion

#pragma region Setup

int calculateNumber() {
   return 1 + rand() % 9; /// @todo algorithm
}

void Grid::init() {
   Storage::numbersCleared.fill(true);
   int value;
   for(size_t row = 0; row < m_grid.size(); row++) {
      for(size_t col = 0; col < m_grid.at(row).size(); col++) {
         if(row < 3 || (row == 3 && col < 5)) // first 3 rows & half of 4th row
            value = calculateNumber();
         else
            value = 0;
         Storage::numbersCleared[value - 1] = false;

         m_grid[row][col].value = value;
         m_grid[row][col].setState(CellState::Rest);
      }
   }
}

void Grid::setCellBounds(GridCell* cell) {
   std::pair<int, int> pos = getCellPos(cell);
   cell->bounds = {
      box.x + pos.second * GridCell::cellSize, // x
      box.y + pos.first * GridCell::cellSize,  // y
      GridCell::cellSize, GridCell::cellSize
   };
}

#pragma endregion

#pragma region Helpers

bool Grid::isCellCompatible(GridCell* cell) const {
   if(!m_focusedCell)
      return false;

   const int cell1 = m_focusedCell->value;
   const int cell2 = cell->value;

   std::pair<int, int> pos1 = getCellPos(m_focusedCell);
   std::pair<int, int> pos2 = getCellPos(cell);
   if(pos1.first > pos2.first)
      std::swap(pos1, pos2); // for more straightforward calculation

   // 1. Value Compatibility
   // cells sum to 10 or are equal but are not empty.
   if( !( (cell1 + cell2 == 10 || cell1 == cell2) && cell1 != 0 ) )
      return false;

   // 2. Same row/column IF no unmatched cell in between
   if(pos1.first == pos2.first) {    // same row
      if(pos1.second > pos2.second)
         std::swap(pos1.second, pos2.second); // in case pos1 col > pos2 col 
      return isRowClear(pos1.first, pos1.second + 1, pos2.second - 1);
   }

   if(pos1.second == pos2.second)   // same column
      return isColClear(pos1.second, pos1.first + 1, pos2.first - 1);

   // 3. Same diagonal IF no unmatched cell in between
   int rowDiff = std::abs(pos2.first - pos1.first);
   int colDiff = std::abs(pos2.second - pos1.second);
   if(rowDiff == colDiff) {  // same diagonal
      int colStep = pos1.second > pos2.second ? -1 : 1;

      for(int row = pos1.first + 1, col = pos1.second + colStep; row != pos2.first; row++, col += colStep)
         if(m_grid.at(row).at(col).getState() != CellState::Matched)
            return false;

      return true;
   }

   // 4. If all cells to the right of cell are matched, its "vision" wraps around to the first cell unmatched cell of next row
   if(rowDiff == 1 && isRowClear(pos1.first, pos1.second + 1))
      return isRowClear(pos2.first, 0, pos2.second - 1);

   return false;
}

bool Grid::isRowClear(int row, int startCol, int endCol) const {
   for(int col = startCol; col <= endCol; col++)
      if(m_grid.at(row).at(col) != 0) // and is not matched, checked by operator
         return false;

   return true;
}

bool Grid::isColClear(int col, int rowStart, int rowEnd) const {
   for(int row = rowStart; row <= rowEnd; row++)
      if(m_grid.at(row).at(col).getState() != CellState::Matched)
         return false;

   return true;
}

bool Grid::isNumClear(int num) const {
   if(Storage::numbersCleared.at(num - 1))
      return true;

   for(const auto& row : m_grid)
      for(const GridCell& cell : row)
         if(cell == num) // operator also checks if both are unmatched
            return false;

   TraceLog(LISHA_SAYS, "%d CLEARED!", num);
   return true;
}

void Grid::clearRow(int row) {
   m_grid.erase(m_grid.begin() + row);

   if(m_grid.size() < 9) { // maintain at least 9 rows
      m_grid.push_back({ 0, 0, 0, 0, 0, 0, 0, 0, 0 });
      for(GridCell& cell : m_grid.back())
         setCellBounds(&cell);
   }
}

#pragma endregion

#pragma region Finders

std::pair<int, int> Grid::getCellPos(GridCell* cell) const {
   for(size_t row = 0; row < m_grid.size(); row++) {
      for(size_t col = 0; col < m_grid.at(row).size(); col++) {
         if(&m_grid.at(row).at(col) == cell)
            return { row, col };
      }
   }

   TraceLog(LOG_ERROR, "Tried to find position of a cell that doesn't exist!\n\tParam address: %p", (void*)cell);
   return { -1, -1 };
}

GridCell* Grid::findHoveredCell() {
   for(auto& row : m_grid)  // Not const because we want to return a non-const pointer
      for(GridCell& cell : row)
         if(CheckCollisionPointRec(GetMousePosition(), cell.bounds))
            return &cell;

   return nullptr;
}

#pragma endregion