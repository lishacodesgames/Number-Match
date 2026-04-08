#pragma once
#include <utility>
#include <vector>
#include <array>
#include "Core/Layer.h"
#include "GUI/Button.h"

enum class GridCellState { Rest, Hovered, Focused };

struct GridCell {
   GridCell(int value = 0) : value(value), cell({0, 0, 0, 0}) {}

   static constexpr float size = 45.0f;
   static constexpr Color restColor = RAYWHITE;
   static Color hoverColor, focusedColor;

   int value; /// 0 for empty, 1-9 otherwise
   Rectangle cell;
   GridCellState state = GridCellState::Rest;
};

class GameLayer : public Core::Layer {
public:
   GameLayer();
   ~GameLayer() = default;

   void OnAttach() override;
   void OnUpdate() override;
   void OnEvent(Core::Event& e) override;
   void OnRender() override;
private:
   // --- grid ---
   Rectangle m_gridBox;

   std::vector<std::array<GridCell, 9>> m_grid; /// Vector of 9-length arrays 
   std::pair<GridCell*, GridCell*> m_focusedCells;

   void initGrid();

   // --- buttons ---
   GUI::Button m_gobackButton;
   GUI::Button m_settingsButton;
   GUI::Button m_plusButton;
   GUI::Button m_hintButton;

   // --- helpers ---
   GUI::Button* findHoveredButton(); /// @return any of the 4 member buttons OR nullptr
   GridCell* findHoveredGridCell();
   inline bool isFocused(GridCell* cell) const {
      return (cell == m_focusedCells.first || cell == m_focusedCells.second);
   }
};

/// adding two gridcells adds their value
inline int operator+(const GridCell& a, const GridCell& b) { return a.value + b.value; }