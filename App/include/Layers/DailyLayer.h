#pragma once
#include "MenuPanel.h"
#include "Event.h"
#include "Layer.h"

class DailyLayer : public Layer {
public:
   DailyLayer();
   ~DailyLayer() override = default;
   void OnEvent(Event& e) override;
   void OnUpdate() override;
   void OnRender() override;

private:
   MenuPanel m_panel;
};