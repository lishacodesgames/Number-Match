#pragma once
#include <utility>
#include <vector>
#include <array>
#include "GUI/ScrollBar.h"
#include "Colors.h"

enum class Hint { Idk, SameRow, SameColumn, SameDiagonal, VisionWrap };
enum class CellState { Rest, Hovered, Focused, Matched };
enum class MatchType {
   NoMatch = 0,
   Adjacent = 1, Far = 4,
   ClearedNum = 5, ClearedRow = 10, ClearedStage = 150
};

struct HintHighlight
{
   bool isHighlighted = false;
   std::pair<int, int> first{ -1, -1 };
   std::pair<int, int> second{ -1, -1 };
};

struct GridCell {
   GridCell(int value = 0) : value(value), bounds({ 0, 0, 0, 0 }) {}

   static float cellSize, numHeight;
   Color bgColor = Palette::off_bright_bg, numColor = Palette::gridnum_not_matched;

   bool isHighlighted = false;
   int value;  /// 0 for empty, 1-9 otherwise
   Rectangle bounds;

   /// only set if current state is not CellState::Matched
   void setState(CellState newState);
   CellState getState() const { return m_state; }

private:
   CellState m_state = CellState::Rest;

public:
   friend bool operator==(const GridCell& a, int b);
   friend bool operator!=(const GridCell& a, int b);
   friend bool operator==(const GridCell& a, const GridCell& b);
};

class Grid {
public:
   Rectangle box;
   Grid();

public:
   // --- methods ---
   bool OnClick(); /// @return e.Handled to GameLayer
   void Update();
   void Draw();

   // --- gameplay ---
   /**
    * @param cell the cell matching weith m_focusedCell 
    * @param match the type of match between cell and m_focusedCell
    */
   void handleMatch(GridCell* cell, MatchType match);

   void resize();
   void plus();
   bool hint(); /// @return if match exists in grid
private:
   HintHighlight m_hint;
   void hintReset();

private:
   std::vector<std::array<GridCell, 9>> m_grid;  /// Vector of 9-length arrays
   GridCell* m_focusedCell;

   float m_scrollOffset = 0.0f;
   ScrollBar m_scrollBar;
private:
   // --- setup ---
   void init(); /// initialises the VALUES of cells and numbersCleared
   void setCellBounds(GridCell* cell); /// set cell origins to match box
   
   // --- helpers ---
   /** @brief
    * Value compatibility: if sum to 10 OR same

    * Cell compatibility:
    * - Same row/column IF no unmatched cell in between \
    * - Same diagonal IF no unmatched cell in between \
    * - If all cells to the right of cell are matched, its "vision" wraps around to the first unmatched cell of next row
    * 
    * @param cell1 m_focusedCell if not given otherwise
    * @param hint relationship hint btwn cell1 & cell2
    *
    * @return score of match
    */
   MatchType getMatchType(GridCell* cell2, GridCell* cell1 = nullptr, Hint hint = Hint::Idk) const;

   /// @return true IF all cells in row, btwn col limits, are either matched or empty (value = 0)
   bool isRowClear(int row, int startCol = 0, int endCol = 8) const;
   bool isColClear(int col, int rowStart, int rowEnd) const;
   bool isNumClear(int num) const; /// @return true IF no instance of num found in grid
   void clearRow(int row);

   // --- finders ---
   std::pair<int, int> getCellPos(GridCell* cell) const;  /// @return {row, col} of cell in m_grid
   std::vector<GridCell*> getValidCells(); /// @return all unmatched cells
public:
   GridCell* findHoveredCell();
};

inline bool operator==(Hint a, Hint b) {
   return
      static_cast<int>(a) == static_cast<int>(b) ||
      static_cast<int>(a) == static_cast<int>(Hint::Idk) ||
      static_cast<int>(b) == static_cast<int>(Hint::Idk);
}

inline int operator+(const GridCell& a, const GridCell& b) { return a.value + b.value; }

inline bool operator==(const GridCell& a, int b) { return a.value == b && a.m_state != CellState::Matched; }
inline bool operator!=(const GridCell& a, int b) { return a.value != b && a.m_state != CellState::Matched; }

inline bool operator==(const GridCell& a, const GridCell& b) {
   return
      a.value == b.value &&
      a.m_state != CellState::Matched &&
      b.m_state != CellState::Matched;
}