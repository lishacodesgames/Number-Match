#pragma once
#include "PanelLayer.h"
#include "Core/Event.h"
#include "Core/Layer.h"

class MeLayer : public Core::Layer {
public:
   MeLayer();
   ~MeLayer() override = default;
   void OnAttach() override;
   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;
};