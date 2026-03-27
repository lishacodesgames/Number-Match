#pragma once
#include <array>
#include <vector>
#include "Core/Layer.h"
#include "GUI/Button.h"

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
   std::vector<std::array<int, 9>> m_grid; /// Vector of 9-length arrays, each with number 0-9. 0 = empty cell. 
   Rectangle m_gridBox;
   void initGrid();

   // --- buttons ---
   GUI::Button m_gobackButton;
   GUI::Button m_settingsButton;
   GUI::Button m_plusButton;
   GUI::Button m_hintButton;

   // --- helpers ---
   GUI::Button* findHoveredButton();
};