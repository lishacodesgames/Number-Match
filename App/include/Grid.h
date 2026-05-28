#pragma once
#include <raylib.h>
#include <utility>
#include <vector>
#include <array>

enum class CellState { Rest, Hovered, Focused, Matched };

struct GridCell {
   GridCell(int value = 0) : value(value), bounds({0, 0, 0, 0}) {}

   static float size;
   static constexpr Color restColor = RAYWHITE;
   static Color hoverColor, focusedColor, matchedColor;

   int value; /// 0 for empty, 1-9 otherwise
   Rectangle bounds;

   /// @param newState only set if current state is not CellState::Matched
   inline void setState(CellState newState) { 
      if(state != CellState::Matched)
         state = newState;
   }
   inline CellState getState() const { return state; }
private:
   CellState state = CellState::Rest;
};

struct Grid {
   std::vector<std::array<GridCell, 9>> grid{}; /// Vector of 9-length arrays 
   GridCell* focusedCell;
   Rectangle box{};

   Grid();
   void Draw();
   
   // --- gameplay ---
   void resize();
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

   // --- iteration ---
   inline auto begin() { return grid.begin(); }
   inline auto end() { return grid.end(); }

   // --- helpers ---
   std::pair<int, int> getCellPos(GridCell* cell) const; /// @return {row, col} of cell in grid
};


inline int operator+(const GridCell& a, const GridCell& b) { return a.value + b.value; }

inline bool operator==(const GridCell& a, const GridCell& b) { return a.value == b.value; }
inline bool operator==(const GridCell& a, int b) { return a.value == b; }