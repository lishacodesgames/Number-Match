#pragma once
#include "Core/Event.h" 
#include "Core/Layer.h"
#include "GUI/Button.h"

enum class Menu {None, Home, Daily, Me};

class PanelLayer : public Layer {
   static PanelLayer* s_instance; // instance of panel layer, so other layers can access panel without dependencies 
public:
   static void PopInstance();

   PanelLayer();
   ~PanelLayer() override;
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