#pragma once
#include "Core/Event.h" 
#include "Core/Layer.h"
#include "GUI/Button.h"
#include "App.h"

enum class Menu {None, Home, Daily, Me};

class PanelLayer : public Core::Layer {
public:
   PanelLayer();
   ~PanelLayer() override { currentPage = Menu::None; }

   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;
public:   
   static constexpr int HEIGHT = 50;
   static constexpr float BUTTON_SPACING = 192.0f;

   GUI::Button homeButton;
   GUI::Button dailyButton;
   GUI::Button meButton;

   Core::Layer* currentLayer;
   static Menu currentPage;
public:
   GUI::Button* findHoveredButton();
   GUI::Button* findFocusedButton();

   void resetAllFocus();
   void setButtonsOrigin();
};