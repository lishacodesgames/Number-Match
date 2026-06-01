#pragma once
#include <raylib.h>
#include "Core/Event.h"
#include "Core/Layer.h"
#include <string>
#include <utility>

class CoinLayer : public Core::Layer {
public:
   CoinLayer();
   ~CoinLayer() override { UnloadTexture(m_coinTexture); }

   void OnEvent([[maybe_unused]] Core::Event& e) override {}
   void OnUpdate() override;
   void OnRender() override;

   static Rectangle box;
private:
   Image m_coinImage = {0};
   Texture m_coinTexture = {0};
   float m_fontSize;

   void resize();
};