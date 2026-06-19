#pragma once
#include <raylib.h>
#include "Core/Layer.h"
#include "GUI/Button.h"
#include "Grid.h"

class GameLayer : public Core::Layer {
public:
   GameLayer();
   ~GameLayer() override {
      UnloadTexture(m_trophyTexture);
      UnloadTexture(m_tickTexture);
   }

   void OnAttach() override {
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
      Core::Layer::OnAttach();
   }

   void OnUpdate() override;
   void OnEvent(Core::Event& e) override;
   void OnRender() override;

   void OnResume() override {
      resize();
      Layer::OnResume();
   }

private:
   // --- gameplay ---
   Grid m_grid;
   void handleMatch(GridCell* cell);  /// handles matching with focused cell

   // --- game info ---
   const Texture m_trophyTexture, m_tickTexture;
   float m_trophyScale, m_tickScale;

   // --- buttons ---
   GUI::Button m_gobackButton;
   GUI::Button m_settingsButton;
   GUI::Button m_plusButton;
   GUI::Button m_hintButton;

private:
   void resize();
   GUI::Button* findHoveredButton(); /// @return any of the 4 member buttons OR nullptr
};

