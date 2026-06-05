#pragma once
#include <raylib.h>
#include "Core/Layer.h"

class DailyLayer : public Core::Layer {
public:
   DailyLayer() : Layer("Daily Layer") {}
   ~DailyLayer() override = default;

   void OnAttach() override {
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
      Core::Layer::OnAttach();
   }

   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;
};