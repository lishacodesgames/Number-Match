#pragma once

#pragma once
#include <Layer.h>
#include <Event.h>
#include "MenuPanel.h"

class MeLayer : public Layer {
public:
   MeLayer();
   ~MeLayer() override = default;
   void OnEvent(Event& e) override;
   void OnUpdate() override;
   void OnRender() override;

private:
   MenuPanel m_panel;
};