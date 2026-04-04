#pragma once
#include <utility>
#include <vector>
#include <array>
#include "Core/Layer.h"
#include "GUI/Button.h"

struct GridCell {
   static constexpr float size = 45.0f;

   int value; /// 0 for empty, 1-9 otherwise
   Rectangle cell;
   bool isHovered = false;
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
   std::vector<std::array<GridCell, 9>> m_grid; /// Vector of 9-length arrays 
   Rectangle m_gridBox;
   void initGrid();

   // --- buttons ---
   GUI::Button m_gobackButton;
   GUI::Button m_settingsButton;
   GUI::Button m_plusButton;
   GUI::Button m_hintButton;

   // --- helpers ---
   GUI::Button* findHoveredButton();
   void highlightHoveredCell();
};