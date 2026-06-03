#pragma once
#include <raylib.h>
#include "Core/Layer.h"
#include "GUI/Button.h"
#include "Grid.h"

class GameLayer : public Core::Layer {
public:
   GameLayer(bool reset); /// @param reset : if true, resets the storage saves, otherwise loads game layer with existing info.
   ~GameLayer() = default;

   void OnAttach() override {
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
      Core::Layer::OnAttach();
   }

   void OnUpdate() override;
   void OnEvent(Core::Event& e) override;
   void OnRender() override;

   void OnResume() override;
private:
   // --- gameplay ---
   Grid m_grid;
   void handleMatch(GridCell* cell);  /// handles matching with focused cell

   // --- game info ---
   Texture m_trophyTexture = {0};
   Texture m_tickTexture = {0};

   // --- buttons ---
   GUI::Button m_gobackButton;
   GUI::Button m_settingsButton;
   GUI::Button m_plusButton;
   GUI::Button m_hintButton;

private:
   void resize();
   GUI::Button* findHoveredButton(); /// @return any of the 4 member buttons OR nullptr
};

