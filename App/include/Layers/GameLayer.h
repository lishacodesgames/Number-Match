#pragma once
#include "Core/Layer.h"
#include "GUI/Button.h"

class GameLayer : public Layer {
public:
   GameLayer();
   ~GameLayer() = default;

   void OnAttach() override;
   void OnUpdate() override;
   void OnEvent(Event& e) override;
   void OnRender() override;
private:
   Button m_gobackButton;
   Button m_settingsButton;
   Button m_plusButton;

   // --- helpers ---
   Button* findHoveredButton();
};