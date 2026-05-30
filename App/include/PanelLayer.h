#pragma once
#include "Core/Event.h" 
#include "Core/Layer.h"
#include "GUI/Button.h"
#include "App.h"

enum class Menu {None, Home, Daily, Me};

class PanelLayer : public Core::Layer {
public:
   PanelLayer();
   ~PanelLayer() override = default;

   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;
public:   
   static constexpr int HEIGHT = 50;
   static constexpr float BUTTON_SPACING = 192.0f;
   
   Core::Layer* currentLayer;

   GUI::Button homeButton;
   GUI::Button dailyButton;
   GUI::Button meButton;
public:
   void setButtonsOrigin();
   GUI::Button* findHoveredButton() {
      if (homeButton.isHovered)
         return &homeButton;
      else if (dailyButton.isHovered)
         return &dailyButton;
      else if (meButton.isHovered)
         return &meButton;
      else
         return nullptr;
   }
};