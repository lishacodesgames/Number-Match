#include <pch/Precompiled.h>
#include "Grid.h"

#include "Layers/CoinLayer.h"
#include "Colors.h"
#include "App.h"

float GridCell::cellSize = 45.0f;
float GridCell::numHeight = 0;

Grid::Grid() : focusedCell(nullptr),
      m_scrollBar({ 0, 0, 0, 0 }, 10.0f, GRID_SCROLL_THUMB, GRID_SCROLL_TRACK)
{
   m_grid.assign(9, { 0, 0, 0, 0, 0, 0, 0, 0, 0 }),  // 9 rows of all blank cells
   srand(time(0));

   for(size_t row = 0; row < m_grid.size(); row++) {
      for(size_t col = 0; col < m_grid.at(row).size(); col++) {
         if(row < 3 || (row == 3 && col < 5)) // first 3 rows & half of 4th row
            m_grid[row][col].value = 1 + rand() % 9;
         else
            m_grid[row][col].value = 0;

         m_grid[row][col].setState(CellState::Rest);
      }
   }

   resize();
   m_scrollBar.setTrackBounds({ box.x + box.width + 25, box.y, 2, box.height });
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
               numColor = GRIDCELL_NOT_MATCHED;
               bgColor = OFF_BRIGHT_BG;
               break;
            case CellState::Hovered:
               numColor = GRIDCELL_NOT_MATCHED;
               bgColor = GRIDCELL_HOVER;
               break;
            case CellState::Focused:
               numColor = GRIDCELL_NOT_MATCHED;
               bgColor = GRIDCELL_FOCUS;
               break;
            case CellState::Matched:
               numColor = GRIDCELL_MATCHED;
               bgColor = OFF_BRIGHT_BG;
               break;
         }

         DrawRectangleRec(cell.bounds, bgColor);
         DrawRectangleLinesEx(cell.bounds, 1, SHADOW_FOR_OFF_BRIGHT);
         DrawTextEx(
            App::font_semibold, value.c_str(),
            {  cell.bounds.x + (cell.bounds.width - numSize.x) / 2, 
               cell.bounds.y + (cell.bounds.height - numSize.y) / 2 },
            GridCell::numHeight, 1, numColor
         );
      }
   }
   EndScissorMode();

   DrawRectangleLinesEx(box, 3, GRIDBOX_COLOR);
   if(m_grid.size() > 9)
      m_scrollBar.Draw();
}

void Grid::resize() {
   float old = GridCell::cellSize;
   GridCell::cellSize = std::clamp( // scaled based on smaller of the 2 screen dimensions
      std::min(GetScreenWidth(), GetScreenHeight()) * 0.7f / 9, 40.0f, 150.0f
   );
   GridCell::numHeight = GridCell::cellSize * 0.75f;

   if(std::abs(old - GridCell::cellSize) > 0.5f)
      LOG_RESIZE("Grid resized: cellSize = %f", GridCell::cellSize);

   // box is centered horizontally and has a fixed y value
   float boxX = (GetScreenWidth() - GridCell::cellSize * 9) / 2.0f;
   float boxY = (GetScreenHeight() - GridCell::cellSize * (9.0f - 1.35f)) / 2.0f; // 1 extra row for the game info on top

   box = { boxX, boxY, GridCell::cellSize * 9, GridCell::cellSize * 9 };

   for(size_t row = 0; row < m_grid.size(); row++)
      for(size_t col = 0; col < m_grid.at(row).size(); col++) 
         setCellBounds(&m_grid[row][col]);

   m_scrollBar.setTrackBounds({ box.x + box.width + 25, box.y, 2, box.height });
}

void Grid::plus() {
   std::vector<int> plusValues;
   for(size_t row = 0; row < m_grid.size(); row++)
      for(size_t col = 0; col < m_grid.at(row).size(); col++)
         if(m_grid[row][col].value != 0 && m_grid[row][col].getState() != CellState::Matched)
            plusValues.push_back(m_grid[row][col].value);

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

void Grid::clearRow(int row) {
   m_grid.erase(m_grid.begin() + row);

   if(m_grid.size() < 9) { // maintain at least 9 rows
      m_grid.push_back({ 0, 0, 0, 0, 0, 0, 0, 0, 0 });
      for(GridCell& cell : m_grid.back())
         setCellBounds(&cell);
   }
}

bool Grid::isRowClear(int row) {
   for(const GridCell& cell : m_grid.at(row)) {
      if(cell.getState() != CellState::Matched && cell.value != 0)
         return false;
   }
   return true;
}

void Grid::setCellBounds(GridCell* cell) {
   std::pair<int, int> pos = getCellPos(cell);
   cell->bounds = {
      box.x + pos.second * GridCell::cellSize,  // x
      box.y + pos.first * GridCell::cellSize,  // y
      GridCell::cellSize, GridCell::cellSize
   };
}

std::pair<int, int> Grid::getCellPos(GridCell* cell) const {
   for(size_t row = 0; row < m_grid.size(); row++) {
      for(size_t col = 0; col < m_grid.at(row).size(); col++) {
         if(&m_grid[row][col] == cell)
            return { row, col };
      }
   }

   return { -1, -1 };  // default case (should not happen)
}

bool Grid::isCellCompatible(std::pair<int, int> pos) const {
   if(!focusedCell)
      return false;

   GridCell cell1 = *focusedCell;
   GridCell cell2 = m_grid.at(pos.first).at(pos.second);
   std::pair<int, int> pos1 = getCellPos(focusedCell);
   std::pair<int, int> pos2 = pos;

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

      for(int row = rowStart, col = colStart; row != pos2.first; row += rowStep, col += colStep)
         if(m_grid.at(row).at(col).getState() != CellState::Matched)
            return false;  // if there is a non-matched cell in between, the cells are not compatible

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

   return false;
}

GridCell* Grid::findHoveredCell() {
   for(auto& row : m_grid) {  // Not const because we want to return a non-const pointer
      for(GridCell& cell : row) {
         if(CheckCollisionPointRec(GetMousePosition(), cell.bounds))
            return &cell;
      }
   }
   return nullptr;
}
