#pragma once
#include <raylib.h>
#include "Layers/PanelLayer.h"
#include "Button.h"
#include "Layer.h"

class HomeLayer : public Layer {
public:
   HomeLayer();
   ~HomeLayer() override;
   void OnUpdate() override;
   void OnEvent(Event &e) override;
   void OnRender() override;

private:
   Texture2D m_backgroundTexture;   
   Button m_newButton;
   Button m_continueButton;

   Button* findHoveredButton();
};