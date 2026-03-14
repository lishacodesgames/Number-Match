#pragma once
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
   GUI::Button m_gobackButton;
   GUI::Button m_settingsButton;
   GUI::Button m_plusButton;

   // --- helpers ---
   GUI::Button* findHoveredButton();
};