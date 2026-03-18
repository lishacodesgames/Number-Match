#pragma once
#include <raylib.h>
#include "Core/Event.h"
#include "Core/Layer.h"

class CoinLayer : public Core::Layer {
public:
   CoinLayer();
   ~CoinLayer() override;

   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;
private:
   Texture m_coinTexture = {0};
};