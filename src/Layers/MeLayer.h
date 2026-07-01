#pragma once
#include "Core/Layer.h"

class MeLayer : public Core::Layer {
public:
   MeLayer() : Core::Layer("Me Layer") {}
   ~MeLayer() override = default;

   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;
};