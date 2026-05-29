#pragma once
#include <raylib.h>
#include "Core/Layer.h"
#include "GUI/Button.h"
#include "Grid.h"

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
   Grid m_grid;

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

   GUI::Button* findHoveredButton(); /// @return any of the 4 member buttons OR nullptr
};

