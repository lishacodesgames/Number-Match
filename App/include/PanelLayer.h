#pragma once
#include "Core/Event.h" 
#include "Core/Layer.h"
#include "GUI/Button.h"

enum class Menu {None, Home, Daily, Me};

class PanelLayer : public Core::Layer {
   static PanelLayer* s_instance; // instance of panel layer, so other layers can access panel without dependencies 
public:
   static void PopInstance();

   PanelLayer();
   ~PanelLayer() override;
   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;
   
   static constexpr float HEIGHT = 50.0f;
   static constexpr float BUTTON_SPACING = 192.0f;

   GUI::Button homeButton;
   GUI::Button dailyButton;
   GUI::Button meButton;

   Menu currentPage;
   Core::Layer* currentLayer;

   GUI::Button* findHoveredButton();
   GUI::Button* findActiveButton();
   GUI::Button* findFocusedButton();

   void resetAllFocus();
};