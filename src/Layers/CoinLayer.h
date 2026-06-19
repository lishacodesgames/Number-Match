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
   const Texture m_coinTexture;
   float m_fontSize, m_textureScale;

   void resize();
};