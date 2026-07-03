#include <pch/Precompiled.h>
#include "Grid.h"

#include "Storage.h"
#include "Colors.h"
#include "App.h"

#define NINE_ZEROES { 0, 0, 0, 0, 0, 0, 0, 0, 0 }

float GridCell::cellSize = 45.0f;
float GridCell::numHeight = 0.0f;

Grid::Grid(bool reset) :
      m_scrollBar({ 0, 0, 0, 0 }, 10.0f, Palette::grid_scroll_thumb, Palette::grid_scroll_track)
{
   srand(time(0));

   if(reset)
      init();
   else {
      const auto savedGrid = Storage::getSavedGrid();
      m_grid.assign(savedGrid.size(), NINE_ZEROES);
      for(size_t row = 0; row < savedGrid.size(); row++) {
         for(size_t col = 0; col < savedGrid.at(row).size(); col++) {
            GridCell& cell = m_grid[row][col];
            const auto& [value, state] = savedGrid.at(row).at(col);

            cell.value = value; 
            cell.setState(state == "Matched" ? CellState::Matched : CellState::Rest);
         }
      }
   }

   resize();
   m_scrollBar.setTrackBounds({ box.x + box.width + 25, box.y, 2, box.height });
}

#pragma region Methods

bool Grid::OnClick() {
   static float timeSinceLastCall = 0.0f;
   if(m_hint.isHighlighted && timeSinceLastCall >= 60 * GetFrameTime())
      m_hint.reset(m_grid);
   timeSinceLastCall = 0.0f;

   GridCell* activeCell = findHoveredCell();
   GridCell* focusedCell = m_focusedCell ? getCellAt(*m_focusedCell) : nullptr;
   if(activeCell) {
      const CellPosition activePosition = getCellPos(activeCell);
      if(!m_focusedCell || activePosition != *m_focusedCell) {  // new cell was clicked
         MatchType match = getMatchType(activeCell);
         if(static_cast<bool>(match)) {
            handleMatch(activeCell, match);
         } else {
            if(focusedCell)
               focusedCell->setState(CellState::Rest);

            if(*activeCell != 0) { // empty cells cannot be focused
               m_focusedCell = activePosition;
               activeCell->setState(CellState::Focused);
            } else {
               m_focusedCell.reset();
            }
         }
      } else {  // clicking the already focused cell should deselect it
         activeCell->setState(CellState::Hovered);
         m_focusedCell.reset();
      }

      return true;
   } else if(m_focusedCell) {
      // clicking a matched cell or outside of the grid or a matched cell should deselect the cells
      focusedCell->setState(CellState::Rest);
      m_focusedCell.reset();
   }

   return false; // in case the click has to be handled by another layer
}

bool canOverride(GridCell* cell) {
   return cell && cell->getState() != CellState::Focused && cell->getState() != CellState::Matched;
} // we can't override focused or matched cells)   

void Grid::Update() {
   // Update hovered cell's state
   GridCell* hoveredCell = findHoveredCell();
   std::optional<CellPosition> hoveredPosition;
   if(hoveredCell)
      hoveredPosition = getCellPos(hoveredCell);

   if(hoveredPosition != m_hoveredCell) {  // a new cell is hovered
      if(canOverride(hoveredCell))
         hoveredCell->setState(CellState::Hovered);
      if(m_hoveredCell) {
         GridCell* previousCell = getCellAt(*m_hoveredCell);
         if(canOverride(previousCell))
            previousCell->setState(CellState::Rest);
      }
   }
   m_hoveredCell = hoveredPosition;

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

void Grid::Draw() {
   // if theme is changed, we must update the colors
   /// @todo I don't like this. Pls fix
   static bool previous = Storage::ui.isDarkMode;
   BeginScissorMode(box.x, box.y, box.width, box.height); // so grid cells don't render outside of the box
   for(size_t row = 0; row < m_grid.size(); row++)
      for(size_t col = 0; col < m_grid.at(row).size(); col++) {
         GridCell& cell = m_grid[row][col];
         const std::string value = cell.value ? std::to_string(cell.value) : "";  // empty string if value is 0 (empty cell)
         const Vector2 numSize = MeasureTextEx(App::font_semibold, value.c_str(), GridCell::numHeight, 1);

         if(previous != Storage::ui.isDarkMode)
            cell.setState(cell.getState());

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
   previous = Storage::ui.isDarkMode;

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
      LOG_RESIZE("Grid cellSize -> {}", GridCell::cellSize);

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
   const std::vector<int> values = getValidValues();
   if(values.empty()) {
      Core::ConsoleLog(LOG_ERROR, "There are no cells in the grid!");
      return;
   }

   if(m_focusedCell) {
      if(GridCell* focusedCell = getCellAt(*m_focusedCell); focusedCell)
         focusedCell->setState(CellState::Rest);
      m_focusedCell.reset();
   }
   m_hoveredCell.reset();
   m_hint.reset(m_grid);

   size_t firstEmptyIndex = m_grid.size() * 9;
   for(size_t row = 0; row < m_grid.size(); row++) {
      for(size_t col = 0; col < m_grid.at(row).size(); col++) {
         if(m_grid.at(row).at(col) == 0) {
            firstEmptyIndex = row * 9 + col;
            row = m_grid.size(); // to break out of outer loop
            break;
         }
      }
   }

   const size_t requiredCells = firstEmptyIndex + values.size();
   const size_t requiredRows = std::max<size_t>(9, (requiredCells + 8) / 9);
   m_grid.resize(requiredRows, NINE_ZEROES);

   for(size_t i = 0; i < values.size(); i++) {
      const size_t target = firstEmptyIndex + i;
      GridCell& cell = m_grid[target / 9][target % 9];
      cell.value = values.at(i);
      cell.isHighlighted = false;
      cell.setState(CellState::Rest);
   }
   resize();
}

bool Grid::hint() {
   // same row
   for(size_t row = 0; row < m_grid.size(); row++) {
      if(m_grid.at(row).at(0) == 0)
         break;

      for(size_t col = 0; col < 9 - 1; col++) {
         auto cell1 = m_grid.at(row).begin() + col;
         if(*cell1 == 0)
            break;
         if(cell1->getState() == CellState::Matched)
            continue;

         auto it = std::find_if(cell1 + 1, m_grid.at(row).end(),
                     [this, cell1](GridCell& cell2) -> bool {
                        return
                           cell2.getState() != CellState::Matched &&
                           this->getMatchType(&cell2, &(*cell1), HintType::SameRow) != MatchType::NoMatch;
                     }
                  );

         if(it != m_grid.at(row).end()) {
            m_hint.isHighlighted = true;

            m_hint.first = { static_cast<int>(row), static_cast<int>(col) };
            cell1->isHighlighted = true;
            cell1->setState(CellState::Rest);

            m_hint.second = {
               static_cast<int>(row), static_cast<int>(std::distance(m_grid.at(row).begin(), it)) };
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

      for(size_t col = 0; col < 9; col++) {
         GridCell& cell1 = m_grid[row1][col];
         if(cell1 == 0)
            break;
         if(cell1.getState() == CellState::Matched)
            continue;
            
         for(size_t row2 = row1 + 1; row2 < m_grid.size(); row2++) {
            GridCell& cell2 = m_grid[row2][col];
            if(cell2 == 0)
               break;
            if(cell2.getState() == CellState::Matched)
               continue;

            if(getMatchType(&cell2, &cell1, HintType::SameColumn) != MatchType::NoMatch) {
               m_hint.isHighlighted = true;

               m_hint.first = { static_cast<int>(row1), static_cast<int>(col) };
               cell1.isHighlighted = true;
               cell1.setState(CellState::Rest);

               m_hint.second = { static_cast<int>(row2), static_cast<int>(col) };
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
   for(size_t row1 = 0; row1 < m_grid.size() - 1; row1++) {
      if(m_grid.at(row1).at(0) == 0)
         break;

      for(size_t col1 = 0; col1 < 9; col1++) {
         GridCell& cell1 = m_grid[row1][col1];
         if(cell1 == 0)
            break;
         if(cell1.getState() == CellState::Matched)
            continue;
            
         for(int colStep = -1; colStep <= 1; colStep += 2) { // -1 for -ve diagonal, and vice versa
            for(int row2 = row1 + 1, col2 = col1 + colStep;
                  col2 >= 0 && col2 < 9 && row2 < static_cast<int>(m_grid.size()); row2++, col2 += colStep) {

               GridCell& cell2 = m_grid[row2][col2];
               if(cell2 == 0)
                  break;
               if(cell2.getState() == CellState::Matched)
                  continue;

               if(getMatchType(&cell2, &cell1, HintType::SameDiagonal) != MatchType::NoMatch) {
                  m_hint.isHighlighted = true;

                  m_hint.first = { static_cast<int>(row1), static_cast<int>(col1) };
                  cell1.isHighlighted = true;
                  cell1.setState(CellState::Rest);

                  m_hint.second = { static_cast<int>(row2), static_cast<int>(col2) };
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
   for(size_t row1 = 0; row1 < m_grid.size() - 1; row1++) {
      if(m_grid.at(row1).at(0) == 0)
         break;

      size_t row2 = row1 + 1;

      for(size_t col1 = 0; col1 < 9; col1++) {
         GridCell& cell1 = m_grid[row1][col1];
         if(cell1 == 0)
            break;
         if(cell1.getState() == CellState::Matched)
            continue;
            
         for(size_t col2 = 0; col2 < 9; col2++) {
            GridCell& cell2 = m_grid[row2][col2];
            if(cell2 == 0)
               break;
            if(cell2.getState() == CellState::Matched)
               continue;

            if(getMatchType(&cell2, &cell1, HintType::VisionWrap) != MatchType::NoMatch) {
               m_hint.isHighlighted = true;

               m_hint.first = { static_cast<int>(row1), static_cast<int>(col1) };
               cell1.isHighlighted = true;
               cell1.setState(CellState::Rest);

               m_hint.second = { static_cast<int>(row2), static_cast<int>(col2) };
               cell2.isHighlighted = true;
               cell2.setState(CellState::Rest);

               return true;
            }
            
            // found an unmatched cell that wasn't compatible, move onto next diagonal / cell
            break;
         }
      }
   }

   // no matches found
   m_hint.isHighlighted = true;
   return false;
}

void Grid::handleMatch(GridCell* cell, MatchType match) {
   if(!static_cast<bool>(match)) {
      Core::ConsoleLog(LOG_ERROR, "Tried to handle a NoMatch!");
      return;
   }

   GridCell* focusedCell = m_focusedCell ? getCellAt(*m_focusedCell) : nullptr;
   if(!focusedCell) {
      Core::ConsoleLog(LOG_ERROR, "Tried to handle a match without a focused cell!");
      return;
   }

   focusedCell->setState(CellState::Matched);
   cell->setState(CellState::Matched);
   Storage::game.currentScore += static_cast<int>(match) * Storage::game.stage;
   
   int num1 = focusedCell->value;
   int num2 = cell->value;
   
   // check if either cell's number is clear
   if(isNumClear(num1)) {
      Storage::game.numbersCleared[num1 - 1] = true;
      Storage::game.currentScore += static_cast<int>(MatchType::ClearedNum) * Storage::game.stage;
   }

   if(num1 != num2 && isNumClear(num2)) {
      Storage::game.numbersCleared[num2 - 1] = true;
      Storage::game.currentScore += static_cast<int>(MatchType::ClearedNum) * Storage::game.stage;
   }

   // check if either cell's row is clear
   int row1 = getCellPos(focusedCell).row;
   int row2 = getCellPos(cell).row;
   const bool sameRow = row1 == row2;
   m_focusedCell.reset();
   m_hoveredCell.reset();

   if(isRowClear(row1)) {
      clearRow(row1);
      Storage::game.currentScore += static_cast<int>(MatchType::ClearedRow) * Storage::game.stage;

      if(row2 > row1)
         row2--;
   }
  
   if(!sameRow && isRowClear(row2)) {
      clearRow(row2);
      Storage::game.currentScore += static_cast<int>(MatchType::ClearedRow) * Storage::game.stage;
   }

   if(m_grid.at(0).at(0) == 0) { // grid is empty
      Core::ConsoleLog(LISHA_SAYS, std::format("Stage {} COMPLETED!", Storage::game.stage));
      Storage::game.currentScore += static_cast<int>(MatchType::ClearedStage) * Storage::game.stage;
      Storage::game.stage++;
      init();
   }

   // update best score
   if(Storage::game.bestScore < Storage::game.currentScore)
      Storage::game.bestScore = Storage::game.currentScore;
}

#pragma endregion

#pragma region Setup

int calculateNumber() {
   return 1 + rand() % 9; /// @todo algorithm
}

void Grid::init() {
   m_focusedCell.reset();
   m_hoveredCell.reset();
   m_hint = {};
   m_grid.assign(9, NINE_ZEROES);  // 9 rows of all blank cells
   Storage::game.numbersCleared.fill(true);
   int value;

   for(size_t row = 0; row < m_grid.size(); row++) {
      for(size_t col = 0; col < m_grid.at(row).size(); col++) {
         GridCell& cell = m_grid[row][col];
         if(row < 3 || (row == 3 && col < 5)) { // first 3 rows & half of 4th row
            value = calculateNumber();
            Storage::game.numbersCleared[value - 1] = false;
         } else {
            value = 0;
         }

         cell.value = value;
         cell.setState(CellState::Rest);
         setCellBounds(&cell);
      }
   }
}

void Grid::setCellBounds(GridCell* cell) {
   CellPosition pos = getCellPos(cell);
   cell->bounds = {
      box.x + pos.col * GridCell::cellSize, // x
      box.y + pos.row * GridCell::cellSize,  // y
      GridCell::cellSize, GridCell::cellSize
   };
}

#pragma endregion

#pragma region Helpers

MatchType Grid::getMatchType(GridCell* cell2, GridCell* cell1, HintType hint) const {
   if(!cell1) {
      if(!m_focusedCell)
         return MatchType::NoMatch;
      cell1 = const_cast<GridCell*>(getCellAt(*m_focusedCell));
      if(!cell1)
         return MatchType::NoMatch;
   }

   if(cell1->getState() == CellState::Matched || cell2->getState() == CellState::Matched)
      return MatchType::NoMatch;

   CellPosition pos1 = getCellPos(cell1);
   CellPosition pos2 = getCellPos(cell2);

   // 1. Value Compatibility
   // cells sum to 10 or are equal but are not empty.
   if( !( (*cell1 + *cell2 == 10 || *cell1 == *cell2) && *cell1 != 0 && *cell2 != 0 ) )
      return MatchType::NoMatch;
      
   if(pos1.row > pos2.row)
      std::swap(pos1, pos2); // for more straightforward calculation

   // 2. Same row/column IF no unmatched cell in between
   if(hint == HintType::SameRow) {
      if(pos1.row == pos2.row) {    // same row
         if(pos1.col > pos2.col)
            std::swap(pos1.col, pos2.col); // in case pos1 col > pos2 col 

         if(isRowClear(pos1.row, pos1.col + 1, pos2.col - 1))
            if(pos2.col - pos1.col == 1)
               return MatchType::Adjacent;
            else
               return MatchType::Far;
         else
            return MatchType::NoMatch;
      }
   }

   if(hint == HintType::SameColumn) {
      if(pos1.col == pos2.col) { // same column
         if(isColClear(pos1.col, pos1.row + 1, pos2.row - 1))
            if(pos2.row - pos1.row == 1)
               return MatchType::Adjacent;
            else
               return MatchType::Far;
         else
            return MatchType::NoMatch;
      }
   }

   // 3. Same diagonal IF no unmatched cell in between
   int rowDiff = pos2.row - pos1.row;
   if(hint == HintType::SameDiagonal) {
      int colDiff = std::abs(pos2.col - pos1.col);
      if(rowDiff == colDiff) {  // same diagonal
         int colStep = pos1.col > pos2.col ? -1 : 1;

         for(int row = pos1.row + 1, col = pos1.col + colStep;
               row != pos2.row; row++, col += colStep) {
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
   if(hint == HintType::VisionWrap)
      if(rowDiff == 1 &&
         isRowClear(pos1.row, pos1.col + 1) &&
         isRowClear(pos2.row, 0, pos2.col - 1)
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
      if(m_grid.at(row).at(col) != 0)
         return false;

   return true;
}

bool Grid::isNumClear(int num) const {
   if(Storage::game.numbersCleared.at(num - 1))
      return true;

   for(const auto& row : m_grid)
      for(const GridCell& cell : row)
         if(cell == num) // operator also checks if both are unmatched
            return false;

   Core::ConsoleLog(LISHA_SAYS, std::format("{} CLEARED!", num));
   return true;
}

void Grid::clearRow(int row) {
   m_focusedCell.reset();
   m_hoveredCell.reset();
   m_grid.erase(m_grid.begin() + row);

   if(m_grid.size() < 9) { // maintain at least 9 rows
      m_grid.push_back(NINE_ZEROES);
      for(GridCell& cell : m_grid.back())
         setCellBounds(&cell);
   }
   
   Core::ConsoleLog(LISHA_SAYS, std::format("Row {} CLEARED!", ++row));
}

#pragma endregion

#pragma region Finders

Grid::CellPosition Grid::getCellPos(GridCell* cell) const {
   for(size_t row = 0; row < m_grid.size(); row++) {
      for(size_t col = 0; col < m_grid.at(row).size(); col++) {
         if(&m_grid.at(row).at(col) == cell)
            return { static_cast<int>(row), static_cast<int>(col) };
      }
   }

   Core::ConsoleLog(LOG_ERROR, std::format("Tried to find position of a cell that doesn't exist!\n\tParam address: {}", static_cast<const void*>(cell)));
   return { -1, -1 };
}

GridCell* Grid::getCellAt(CellPosition position) {
   return const_cast<GridCell*>(std::as_const(*this).getCellAt(position));
}

const GridCell* Grid::getCellAt(CellPosition position) const {
   const auto& [row, col] = position;
   if(row < 0 || col < 0 || static_cast<size_t>(row) >= m_grid.size() || col >= 9)
      return nullptr;
   return &m_grid[row][col];
}

std::vector<int> Grid::getValidValues() const {
   std::vector<int> validValues;
   for(const auto& row : m_grid)
      for(const GridCell& cell : row)
         if(cell != 0)
            validValues.push_back(cell.value);

   return validValues;
}

GridCell* Grid::findHoveredCell() {
   for(auto& row : m_grid)  // Not const because we want to return a non-const pointer
      for(GridCell& cell : row)
         if(CheckCollisionPointRec(GetMousePosition(), cell.bounds))
            return &cell;

   return nullptr;
}

Storage::SavedGrid Grid::getSaveData() const {
   Storage::SavedGrid savedGrid;
   savedGrid.reserve(m_grid.size());
   for(const auto& row : m_grid) {
      Storage::SavedRow savedRow;
      for(size_t col = 0; col < row.size(); col++) {
         const GridCell& cell = row.at(col);
         savedRow[col] = {
            cell.value,
            cell.getState() == CellState::Matched ? "Matched" : "Rest"
         };
      }
      savedGrid.push_back(std::move(savedRow));
   }
   return savedGrid;
}

#pragma endregion

void GridCell::setState(CellState newState) {
   if(m_state == CellState::Matched && newState != CellState::Matched)
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

void Grid::Hint::reset(std::vector<std::array<GridCell, 9>>& grid) {
   isHighlighted = false;

   if(first.row != -1 && first.col != -1) {
      grid[first.row][first.col].isHighlighted = false;
      grid[first.row][first.col].bgColor= Palette::off_bright_bg; 
   }
   first = { -1, -1 };

   if(second.row != -1 && second.col != -1) {
      grid[second.row][second.col].isHighlighted = false;
      grid[second.row][second.col].bgColor= Palette::off_bright_bg; 
   }
   second = { -1, -1 };
}
