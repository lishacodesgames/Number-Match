#pragma once
#include "Core/Layer.h"
#include "GUI/Button.h"

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