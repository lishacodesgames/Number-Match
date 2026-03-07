#pragma once
#include <Layer.h>

class GameLayer : public Layer {
public:
   GameLayer();
   ~GameLayer() = default;

   void OnAttach() override;
   void OnDetach() override;
   void OnUpdate() override;
   void OnEvent(Event& e) override;
   void OnRender() override;

private:
   int x = 400, y = 300;
};