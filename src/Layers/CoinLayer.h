#pragma once
#include "Core/Layer.h"

class CoinLayer : public Core::Layer {
public:
   CoinLayer();
   ~CoinLayer() override { UnloadTexture(m_coinTexture); }

   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;

   static Rectangle box;
private:
   const Texture2D m_coinTexture;
   float m_fontSize, m_textureScale;

private:
   void resize();
};