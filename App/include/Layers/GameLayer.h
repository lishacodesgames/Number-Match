#pragma once
#include <Layer.h>

class GameLayer : public Layer {
public:
   /// @todo unstatic-ify
   static bool s_isSuspended;

   GameLayer();
   ~GameLayer() = default;

   void OnAttach() override;
   void OnUpdate() override;
   void OnEvent(Event& e) override;
   void OnRender() override;

   // TODO (also set renderSuspended)
   void OnSuspend() override {}
   void OnResume() override {}

private:
   int x = 400, y = 300;
};