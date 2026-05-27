#pragma once
#include <raylib.h>
#include <utility>
#include <vector>
#include <array>
#include "Core/Layer.h"
#include "GUI/Button.h"

struct GridCell;

class GameLayer : public Core::Layer {
public:
   GameLayer(bool reset); /// @param reset : if true, resets the storage saves, otherwise loads game layer with existing info.
   ~GameLayer() = default;

   void OnAttach() override;
   void OnResume() override;
   void OnUpdate() override;
   void OnEvent(Core::Event& e) override;
   void OnRender() override;
private:
   // --- grid ---
   Rectangle m_gridBox{};

   std::vector<std::array<GridCell, 9>> m_grid; /// Vector of 9-length arrays 
   GridCell* m_focusedCell;

   void initGrid();

   // --- game info ---
   Texture m_trophyTexture = {0};
   Texture m_tickTexture = {0};

   // --- buttons ---
   GUI::Button m_gobackButton;
   GUI::Button m_settingsButton;
   GUI::Button m_plusButton;
   GUI::Button m_hintButton;

   // --- helpers ---
   void setButtonsOrigin();
   void setGridBox();
   void setGridCells();

   GUI::Button* findHoveredButton(); /// @return any of the 4 member buttons OR nullptr

   GridCell* findHoveredGridCell();
   std::pair<int, int> getCellPos(GridCell* cell) const; /// @return {row, col} of cell in grid

   /** @brief
    * Value compatibility: if sum to 10 OR same

    * Cell compatibility:
    * - Same row/column IF no unmatched cell in between \
    * - Same diagonal IF no unmatched cell in between \
    * - If all cells to the right of cell are matched, its "vision" wraps around to the first unmatched cell of next row
    */
   bool areCellsCompatible(std::pair<int, int> pos1, std::pair<int, int> pos2) const;
};

enum class CellState { Rest, Hovered, Focused, Matched };

struct GridCell {
   GridCell(int value = 0) : value(value), cell({0, 0, 0, 0}) {}

   static float size;
   static constexpr Color restColor = RAYWHITE;
   static Color hoverColor, focusedColor, matchedColor;

   int value; /// 0 for empty, 1-9 otherwise
   Rectangle cell;

   /// @param newState only set if current state is not CellState::Matched
   inline void setState(CellState newState) { 
      if(state != CellState::Matched)
         state = newState;
   }
   inline CellState getState() const { return state; }
private:
   CellState state = CellState::Rest;
};


inline int operator+(const GridCell& a, const GridCell& b) { return a.value + b.value; }

inline bool operator==(const GridCell& a, const GridCell& b) { return a.value == b.value; }
inline bool operator==(const GridCell& a, int b) { return a.value == b; }