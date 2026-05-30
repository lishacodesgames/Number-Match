#pragma once
#include <raylib.h>
#include "Core/Event.h"
#include "Core/Layer.h"

class CoinLayer : public Core::Layer {
public:
   CoinLayer() : Core::Layer("Coin Layer", true) { 
      m_coinTexture = LoadTexture("assets/icons/game/coin_20x20.png");
   }
   ~CoinLayer() override { UnloadTexture(m_coinTexture); }

   void OnEvent([[maybe_unused]] Core::Event& e) override {}
   void OnUpdate() override;
   void OnRender() override;
private:
   Texture m_coinTexture = {0};
};