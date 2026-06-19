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
   if(m_hint.isHighlighted)
      hintReset();

   GridCell* activeCell = findHoveredCell();
   if(activeCell) {
      if(activeCell != m_focusedCell) {  // new cell was clicked
         MatchType match = getMatchType(activeCell);
         if(static_cast<bool>(match)) {
            handleMatch(activeCell, match);
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
   BeginScissorMode(box.x, box.y, box.width, box.height); // so grid cells don't render outside of the box
   for(size_t row = 0; row < m_grid.size(); row++)
      for(size_t col = 0; col < m_grid.at(row).size(); col++) {
         const GridCell& cell = m_grid.at(row).at(col);
         const std::string value = cell.value ? std::to_string(cell.value) : "";  // empty string if value is 0 (empty cell)
         const Vector2 numSize = MeasureTextEx(App::font_semibold, value.c_str(), GridCell::numHeight, 1);

         DrawRectangleRec(cell.bounds, cell.bgColor);
         DrawRectangleLinesEx(cell.bounds, 1, Palette::shadow_for_off_bright);
         DrawTextEx(
            App::font_semibold, value.c_str(),
            {  cell.bounds.x + (cell.bounds.width - numSize.x) / 2, 
               cell.bounds.y + (cell.bounds.height - numSize.y) / 2 },
            GridCell::numHeight, 1, cell.numColor
         );
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
   std::pair<int, int> firstEmptyCell = { -1, -1 };
   for(size_t row = 1; row < m_grid.size(); row++) {
      auto it = std::find(m_grid.at(row).begin(), m_grid.at(row).end(), 0);
      if(it != m_grid.at(row).end()) {
         firstEmptyCell = { row, static_cast<int>(std::distance(m_grid.at(row).begin(), it)) };
         break;
      }

      // if no empty cell till last iteration, we make a new row of empty cell
      if(row == m_grid.size() - 1) {
         m_grid.push_back({ 0, 0, 0, 0, 0, 0, 0, 0, 0 });
         firstEmptyCell = { row, 0 };
         break;
      }
   }

   if(firstEmptyCell.first == -1) {
      TraceLog(LOG_ERROR, "Error in Grid::plus(): no empty cell found and failed to create new row");
      return;
   }

   // Duplicate all valid cells starting from firstEmptyCell
   int row = firstEmptyCell.first;
   int col = firstEmptyCell.second;
   for(GridCell* cell : getValidCells()) {
      m_grid[row][col].value = cell->value;

      // increment grid index
      if(++col == 9) {
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

bool Grid::hint() {
   // same row
   for(size_t row = 0; row < m_grid.size(); row++) {
      if(m_grid.at(row).at(0) == 0)
         break;

      for(size_t col = 0; col < m_grid.at(row).size() - 1; col++) {
         auto cell1 = m_grid.at(row).begin() + col;
         if(*cell1 == 0)
            break;
         if(cell1->getState() == CellState::Matched)
            continue;

         auto it = std::find_if(cell1 + 1, m_grid.at(row).end(),
                     [this, cell1](GridCell& cell2) -> bool {
                        return
                           cell2.getState() != CellState::Matched &&
                           this->getMatchType(&cell2, &(*cell1), Hint::SameRow) != MatchType::NoMatch;
                     }
                  );

         if(it != m_grid.at(row).end()) {
            m_hint.isHighlighted = true;

            m_hint.first = { row, col };
            cell1->isHighlighted = true;
            cell1->setState(CellState::Rest);
            m_hint.second = { row, std::distance(m_grid.at(row).begin(), it) };
            it->isHighlighted = true;
            it->setState(CellState::Rest);
            return true;
         }
      }
   }

   // same column
   for(size_t row1 = 0; row1 < m_grid.size(); row1++) {
      if(m_grid.at(row1).at(0) == 0)
         break;

      for(size_t col = 0; col < m_grid.at(row1).size(); col++) {
         GridCell& cell1 = m_grid[row1][col];
         if(cell1 == 0)
            break;
         if(cell1.getState() == CellState::Matched)
            continue;
            
         for(size_t row2 = row1 + 1; row2 < m_grid.size() - 1; row2++) {
            GridCell& cell2 = m_grid[row2][col];
            if(cell2 == 0)
               break;
            if(cell2.getState() == CellState::Matched)
               continue;

            if(getMatchType(&cell2, &cell1, Hint::SameColumn) != MatchType::NoMatch) {
               m_hint.isHighlighted = true;

               m_hint.first = { row1, col };
               cell1.isHighlighted = true;
               cell1.setState(CellState::Rest);
               m_hint.second = { row2, col };
               cell2.isHighlighted = true;
               cell2.setState(CellState::Rest);
               return true;
            }
            
            // found an unmatched cell that wasn't compatible, move onto next column
            break;
         }
      }
   }

   // same diagonal
   for(size_t row1 = 0; row1 < m_grid.size(); row1++) {
      if(m_grid.at(row1).at(0) == 0)
         break;

      for(size_t col1 = 0; col1 < m_grid.at(row1).size(); col1++) {
         GridCell& cell1 = m_grid[row1][col1];
         if(cell1 == 0)
            break;
         if(cell1.getState() == CellState::Matched)
            continue;
            
         for(int colStep = -1; colStep <= 1; colStep += 2) { // -1 for -ve diagonal, and vice versa
            for(size_t row2 = row1 + 1, col2 = col1 + colStep;
                  col2 < 9 && col2 >= 0; row2++, col2 += colStep) {

               GridCell& cell2 = m_grid[row2][col2];
               if(cell2 == 0)
                  break;
               if(cell2.getState() == CellState::Matched)
                  continue;

               if(getMatchType(&cell2, &cell1, Hint::SameDiagonal) != MatchType::NoMatch) {
                  m_hint.isHighlighted = true;

                  m_hint.first = { row1, col1 };
                  cell1.isHighlighted = true;
                  cell1.setState(CellState::Rest);
                  m_hint.second = { row2, col2 };
                  cell2.isHighlighted = true;
                  cell2.setState(CellState::Rest);
                  return true;
               }
               
               // found an unmatched cell that wasn't compatible, move onto next diagonal / cell
               break;
            }
         }
      }
   }

   // vision wrap
   // ..

   // no matches found
   m_hint.isHighlighted = true;
   return false;
}

void Grid::hintReset() {
   m_hint.isHighlighted = false;

   if(m_hint.first.first != -1) {
      m_grid[m_hint.first.first][m_hint.first.second].isHighlighted = false;
      m_grid[m_hint.first.first][m_hint.first.second].bgColor = Palette::off_bright_bg;
   }
   m_hint.first = { -1, -1 };

   if(m_hint.second.first != -1) {
      m_grid[m_hint.second.first][m_hint.second.second].isHighlighted = false;
      m_grid[m_hint.second.first][m_hint.second.second].bgColor = Palette::off_bright_bg;
   }
   m_hint.second = { -1, -1 };
}

void Grid::handleMatch(GridCell* cell, MatchType match) {
   if(!static_cast<bool>(match)) {
      TraceLog(LOG_ERROR, "Tried to handle a NoMatch!");
      return;
   }

   m_focusedCell->setState(CellState::Matched);
   cell->setState(CellState::Matched);
   Storage::currentScore += static_cast<int>(match) * Storage::stage;
   
   int num1 = m_focusedCell->value;
   int num2 = cell->value;
   
   // check if either cell's number is clear
   if(isNumClear(num1)) {
      Storage::numbersCleared[num1 - 1] = true;
      Storage::currentScore += static_cast<int>(MatchType::ClearedNum) * Storage::stage;
   }

   if(num1 != num2 && isNumClear(num2)) {
      Storage::numbersCleared[num2 - 1] = true;
      Storage::currentScore += static_cast<int>(MatchType::ClearedNum) * Storage::stage;
   }

   // check if either cell's row is clear
   int row1 = getCellPos(m_focusedCell).first;
   int row2 = getCellPos(cell).first;

   if(isRowClear(row1)) {
      clearRow(row1);
      Storage::currentScore += static_cast<int>(MatchType::ClearedRow) * Storage::stage;

      if(row2 > row1)
         row2--;
   }
   
   if(isRowClear(row2)) {
      clearRow(row2);
      Storage::currentScore += static_cast<int>(MatchType::ClearedRow) * Storage::stage;
   }

   if(m_grid.at(0).at(0) == 0) { // grid is empty
      Storage::currentScore += static_cast<int>(MatchType::ClearedStage) * Storage::stage;
      Storage::stage++;
      init();
   }

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

MatchType Grid::getMatchType(GridCell* cell2, GridCell* cell1, Hint hint) const {
   if(!cell1) {
      if(!m_focusedCell)
         return MatchType::NoMatch;
      else
         cell1 = m_focusedCell;
   }

   if(cell1->getState() == CellState::Matched || cell2->getState() == CellState::Matched)
      return MatchType::NoMatch;

   std::pair<int, int> pos1 = getCellPos(cell1);
   std::pair<int, int> pos2 = getCellPos(cell2);

   // 1. Value Compatibility
   // cells sum to 10 or are equal but are not empty.
   if( !( (*cell1 + *cell2 == 10 || *cell1 == *cell2) && *cell1 != 0 && *cell2 != 0 ) )
      return MatchType::NoMatch;
      
   if(pos1.first > pos2.first)
      std::swap(pos1, pos2); // for more straightforward calculation

   // 2. Same row/column IF no unmatched cell in between
   if(hint == Hint::SameRow) {
      if(pos1.first == pos2.first) {    // same row
         if(pos1.second > pos2.second)
            std::swap(pos1.second, pos2.second); // in case pos1 col > pos2 col 

         if(isRowClear(pos1.first, pos1.second + 1, pos2.second - 1))
            if(pos2.second - pos1.second == 1)
               return MatchType::Adjacent;
            else
               return MatchType::Far;
         else
            return MatchType::NoMatch;
      }
   }

   if(hint == Hint::SameColumn) {
      if(pos1.second == pos2.second) { // same column
         if(isColClear(pos1.second, pos1.first + 1, pos2.first - 1))
            if(pos2.first - pos1.first == 1)
               return MatchType::Adjacent;
            else
               return MatchType::Far;
         else
            return MatchType::NoMatch;
      }
   }

   // 3. Same diagonal IF no unmatched cell in between
   int rowDiff = pos2.first - pos1.first;
   if(hint == Hint::SameDiagonal) {
      int colDiff = std::abs(pos2.second - pos1.second);
      if(rowDiff == colDiff) {  // same diagonal
         int colStep = pos1.second > pos2.second ? -1 : 1;

         for(int row = pos1.first + 1, col = pos1.second + colStep;
               row != pos2.first; row++, col += colStep) {
            if(m_grid.at(row).at(col).getState() != CellState::Matched)
               return MatchType::NoMatch;
         }

         if(rowDiff == 1)
            return MatchType::Adjacent;
         else
            return MatchType::Far;
      }
   }

   // 4. If all cells to the right of cell are matched,
   // its "vision" wraps around to the first cell unmatched cell of next row
   if(hint == Hint::VisionWrap)
      if(rowDiff == 1 &&
         isRowClear(pos1.first, pos1.second + 1) &&
         isRowClear(pos2.first, 0, pos2.second - 1)
      ) return MatchType::Far;

   return MatchType::NoMatch;
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

   TraceLog(LOG_ERROR,
      "Tried to find position of a cell that doesn't exist!\n\tParam address: %p", (void*)cell);
   return { -1, -1 };
}

std::vector<GridCell*> Grid::getValidCells() {
   std::vector<GridCell*> validCells{};
   for(auto& row : m_grid)  // Not const because we want to return a non-const pointer
      for(GridCell& cell : row)
         if(cell != 0)
            validCells.push_back(&cell);

   return validCells;
}

GridCell* Grid::findHoveredCell() {
   for(auto& row : m_grid)  // Not const because we want to return a non-const pointer
      for(GridCell& cell : row)
         if(CheckCollisionPointRec(GetMousePosition(), cell.bounds))
            return &cell;

   return nullptr;
}

#pragma endregion

void GridCell::setState(CellState newState) {
   if(m_state == CellState::Matched)
      return;

   m_state = newState;
   switch(m_state) {
      case CellState::Rest:
         numColor = Palette::gridnum_not_matched;
         bgColor = isHighlighted ? Palette::grid_hint : Palette::off_bright_bg;
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
}