#pragma once
#include "Core/Layer.h"

#include <raylib.h>

class CoinLayer : public Core::Layer {
public:
   CoinLayer();
   ~CoinLayer() override { UnloadTexture(m_coinTexture); }

   void OnEvent([[maybe_unused]] Core::Event& e) override {}
   void OnUpdate() override;
   void OnRender() override;

   static Rectangle box;
private:
   const Texture2D m_coinTexture;
   float m_fontSize, m_textureScale;

private:
   void resize();
};