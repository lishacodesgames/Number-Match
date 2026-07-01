#pragma once
#include "Core/Layer.h"

class DailyLayer : public Core::Layer {
public:
   DailyLayer() : Layer("Daily Layer") {}
   ~DailyLayer() override = default;

   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;
};