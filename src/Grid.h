#pragma once
#include "GUI/ScrollBar.h"
#include "Colors.h"
#include "Storage.h"

enum class HintType { Idk, SameRow, SameColumn, SameDiagonal, VisionWrap };
enum class CellState { Rest, Hovered, Focused, Matched };
enum class MatchType {
   NoMatch = 0,
   Adjacent = 1, Far = 4,
   ClearedNum = 5, ClearedRow = 10, ClearedStage = 150
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

public:
   int operator+(const GridCell& other) const { return this->value + other.value; }

   bool operator==(const GridCell& other) const {
      return this->value == other.value &&
      this->m_state != CellState::Matched &&
      other.m_state != CellState::Matched;
   }

   bool operator==(int i) const { return this->value == i && m_state != CellState::Matched; }
   bool operator!=(int i) const { return this->value != i && m_state != CellState::Matched; }

private:
   CellState m_state = CellState::Rest;
};

class Grid {
public:
   Rectangle box;

public:
   Grid(bool reset);

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

public:
   GridCell* findHoveredCell();
   Storage::SavedGrid getSaveData() const;

private:
   struct CellPosition {
      int row = -1, col = -1;
      bool operator==(const CellPosition& other) const = default;
   };

   struct Hint {
      bool isHighlighted = false;
      CellPosition first{ -1, -1 };
      CellPosition second{ -1, -1 };

      void reset(std::vector<std::array<GridCell, 9>>& grid);
   };

private:
   Hint m_hint;

   std::vector<std::array<GridCell, 9>> m_grid;  /// Vector of 9-length arrays
   std::optional<CellPosition> m_focusedCell;
   std::optional<CellPosition> m_hoveredCell;

   float m_scrollOffset = 0.0f;
   GUI::ScrollBar m_scrollBar;

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
   MatchType getMatchType(GridCell* cell2, GridCell* cell1 = nullptr, HintType hint = HintType::Idk) const;

   /// @return true IF all cells in row, btwn col limits, are either matched or empty (value = 0)
   bool isRowClear(int row, int startCol = 0, int endCol = 8) const;
   bool isColClear(int col, int rowStart, int rowEnd) const;
   bool isNumClear(int num) const; /// @return true IF no instance of num found in grid
   void clearRow(int row);
   GridCell* getCellAt(CellPosition position);
   const GridCell* getCellAt(CellPosition position) const;

   // --- finders ---
   CellPosition getCellPos(GridCell* cell) const;  /// @return {row, col} of cell in m_grid
   std::vector<int> getValidValues() const; /// @return values of all unmatched cells
};

/// @return true IF both are same or either is HintType::Idk
inline bool operator==(HintType a, HintType b);
