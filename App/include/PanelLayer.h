#pragma once
#include "Core/Event.h" 
#include "Core/Layer.h"
#include "GUI/Button.h"
#include "App.h"

enum class Menu {None, Home, Daily, Me};

class PanelLayer : public Core::Layer {
public:
   PanelLayer();
   ~PanelLayer() override {
      currentPage = Menu::None;
      s_instance = nullptr;
   }

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
   static void PopInstance() { App::QueueLayerPop(s_instance); }

   GUI::Button* findHoveredButton();
   GUI::Button* findFocusedButton();

   void resetAllFocus();
   void setButtonsOrigin();
private:
   static PanelLayer* s_instance; /// for memory management of this layer
};