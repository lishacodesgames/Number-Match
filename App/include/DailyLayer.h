#pragma once
#include "PanelLayer.h"
#include "Core/Event.h"
#include "Core/Layer.h"

class DailyLayer : public Core::Layer {
public:
   DailyLayer();
   ~DailyLayer() override = default;
   void OnAttach() override;
   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;
};