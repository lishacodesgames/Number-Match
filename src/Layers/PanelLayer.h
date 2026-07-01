#pragma once
#include "Core/Layer.h"
#include "GUI/Button.h"

class PanelLayer : public Core::Layer {
public:
   PanelLayer();
   ~PanelLayer() override = default;

   void OnAttach() override;

   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;

public:   
   static int height;
   static float buttonSpacing;

   Core::Layer* currentLayer;

   GUI::Button homeButton;
   GUI::Button dailyButton;
   GUI::Button meButton;

private:
   void resize();
   GUI::Button* findHoveredButton();
};