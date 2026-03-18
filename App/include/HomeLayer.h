#pragma once
#include <raylib.h>
#include "PanelLayer.h"
#include "Core/Layer.h"
#include "GUI/Button.h"

class HomeLayer : public Core::Layer {
public:
   HomeLayer();
   ~HomeLayer() override;
   void OnUpdate() override;
   void OnEvent(Core::Event &e) override;
   void OnRender() override;

private:
   Texture m_backgroundTexture = {0};   
   Texture m_trophyTexture = {0};

   GUI::Button m_newButton;
   GUI::Button m_continueButton;

   GUI::Button* findHoveredButton();
};