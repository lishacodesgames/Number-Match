#pragma once
#include <raylib.h>
#include <Button.h>
#include <Layer.h>
#include "MenuPanel.h"

class HomeLayer : public Layer {
public:
   HomeLayer();
   ~HomeLayer() override;

   void OnAttach() override;
   void OnDetach() override;
   void OnUpdate() override;
   void OnEvent(Event &e) override;
   void OnRender() override;

private:
   Texture2D m_backgroundTexture;   
   Button m_newButton;
   Button m_continueButton;

   MenuPanel m_panel;
   Button* m_focusedPanelButton;

   Button* findHoveredButton();
};