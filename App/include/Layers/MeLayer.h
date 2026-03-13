#pragma once
#include "Layers/PanelLayer.h"
#include "Core/Event.h"
#include "Core/Layer.h"

class MeLayer : public Layer {
public:
   MeLayer();
   ~MeLayer() override = default;
   void OnAttach() override;
   void OnEvent(Event& e) override;
   void OnUpdate() override;
   void OnRender() override;
};