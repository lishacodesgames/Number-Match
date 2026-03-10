#pragma once
#include "Button.h"
#include "Layer.h"

class GameLayer : public Layer {
public:
   GameLayer();
   ~GameLayer() = default;

   void OnAttach() override;
   void OnUpdate() override;
   void OnEvent(Event& e) override;
   void OnRender() override;
private:
   Button gobackButton;
   Button settingsButton;
};