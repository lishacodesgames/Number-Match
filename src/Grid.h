#pragma once
#include <utility>
#include <vector>
#include <array>
#include "GUI/ScrollBar.h"

enum class CellState { Rest, Hovered, Focused, Matched };

struct GridCell {
   GridCell(int value = 0) : value(value), bounds({ 0, 0, 0, 0 }) {}

   static float cellSize, numHeight;
   static constexpr Color restColor = RAYWHITE;
   static Color hoverColor, focusColor;

   int value;  /// 0 for empty, 1-9 otherwise
   Rectangle bounds;

   /// @param newState only set if current state is not CellState::Matched
   void setState(CellState newState) { if(state != CellState::Matched) state = newState; }
   CellState getState() const { return state; }
private:
   CellState state = CellState::Rest;
};

struct Grid {
   GridCell* focusedCell;
   Rectangle box{};

   Grid();
   void Update();
   void Draw() const;

   // --- gameplay ---
   
   void resize();
   void plus();

   void clearRow(int row);
   bool isRowClear(int row); /// @return true IF all cells in row are either matched or empty (value = 0)

   // --- iteration ---
   inline auto begin() { return m_grid.begin(); }
   inline auto end() { return m_grid.end(); }

   // --- helpers ---
   void setCellBounds(GridCell* cell);                    /// set cell origins to match box
   std::pair<int, int> getCellPos(GridCell* cell) const;  /// @return {row, col} of cell in m_grid
   
   GridCell* findHoveredCell();
   /** @brief
    * Value compatibility: if sum to 10 OR same

    * Cell compatibility:
    * - Same row/column IF no unmatched cell in between \
    * - Same diagonal IF no unmatched cell in between \
    * - If all cells to the right of cell are matched, its "vision" wraps around to the first unmatched cell of next row
    *
    * @return true if cell @pos is compatible with focusedCell
    */
   bool isCellCompatible(std::pair<int, int> pos) const;
private:
   ScrollBar m_scrollBar;
   float m_scrollOffset = 0.0f;

   std::vector<std::array<GridCell, 9>> m_grid{};  /// Vector of 9-length arrays
};

inline int operator+(const GridCell& a, const GridCell& b) { return a.value + b.value; }

inline bool operator==(const GridCell& a, const GridCell& b) { return a.value == b.value; }
inline bool operator==(const GridCell& a, int b) { return a.value == b; }