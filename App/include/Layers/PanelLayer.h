#pragma once
#include "Button.h"
#include "Layer.h"
#include "Event.h" 

enum class Menu {None, Home, Daily, Me};

class PanelLayer : public Layer {
public:
   PanelLayer();
   ~PanelLayer() override = default;
   void OnEvent(Event& e) override;
   void OnUpdate() override;
   void OnRender() override;
   
   static constexpr float HEIGHT = 50.0f;
   static constexpr float BUTTON_SPACING = 192.0f;

   Button homeButton;
   Button dailyButton;
   Button meButton;

   Menu currentPage;
   Layer* currentLayer;

   Button* findHoveredButton();
   Button* findActiveButton();
   Button* findFocusedButton();

   void resetAllFocus();
};