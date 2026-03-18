#include <pch/Precompiled.h>
#include "CoinLayer.h"

#include "PanelLayer.h"
#include "App.h"

CoinLayer::CoinLayer() : Core::Layer("Coin Layer", true) {
   m_coinTexture = LoadTexture("assets/icons/game/coin_20x20.png");
}

CoinLayer::~CoinLayer() { UnloadTexture(m_coinTexture); }

void CoinLayer::OnEvent([[maybe_unused]] Core::Event& e) {}

void CoinLayer::OnUpdate() {
   if(PanelLayer::currentPage == Menu::Daily || PanelLayer::currentPage == Menu::Me) 
      App::QueueLayerPop(this);
}

void CoinLayer::OnRender() {
   /// @todo add changeability

   Rectangle coinBox = {static_cast<float>(GetScreenWidth())/2-48, 20, 100, 40};
   DrawRectangleRounded(coinBox, 5.0f, 5, WHITE);
   DrawTexture(m_coinTexture, coinBox.x+10.0f, coinBox.y+10.0f, WHITE);
   DrawTextEx(App::font_semibold, "1,343", {coinBox.x+34.0f, coinBox.y+5.85f}, 27.3f, 1.5f, {0, 0, 0, 175});
}
