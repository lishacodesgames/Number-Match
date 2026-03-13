#pragma once
#include "Layers/PanelLayer.h"
#include "Core/Event.h"
#include "Core/Layer.h"

class DailyLayer : public Layer {
public:
   DailyLayer();
   ~DailyLayer() override = default;
   void OnAttach() override;
   void OnEvent(Event& e) override;
   void OnUpdate() override;
   void OnRender() override;
};