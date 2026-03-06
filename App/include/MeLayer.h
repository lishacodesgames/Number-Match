#pragma once

#pragma once
#include <Layer.h>
#include <Event.h>
#include "MenuPanel.h"

class MeLayer : public Layer {
public:
   MeLayer();
   ~MeLayer() override;
   void OnAttach() override;
   void OnDetach() override;
   void OnEvent(Event& e) override;
   void OnUpdate() override;
   void OnRender() override;

private:
   MenuPanel m_panel;
   Button* m_focusedPanelButton;
};