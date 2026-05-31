#include <pch/Precompiled.h>
#include "CoinLayer.h"

#include "PanelLayer.h"
#include "Storage.h"
#include "Colors.h"
#include "App.h"

CoinLayer::CoinLayer() : Core::Layer("Coin Layer", true) { 
   m_coinImage = LoadImage("assets/icons/game/coin_20x20.png");
   m_coinTexture = LoadTextureFromImage(m_coinImage);
   amount.first = Storage::formatCoins();
   amount.second = GetScreenHeight() / 23.809f;
}

void CoinLayer::OnUpdate() {
   // cannot be in OnEvent since CoinLayer is sometimes on top of PanelLayer so the coin's check happens before layer is switched
   // causing coins to not update when switching from Home to Daily/Me, since PanelLayer is popped after the click event is processed
   // leads to buggy mess lol
   if(App::GetLayerByName("Panel Layer") && !App::GetLayerByName("Home Layer"))
      App::QueueLayerPop(this);
      
   amount.first = Storage::formatCoins();
   if(IsWindowResized())
      resize();
}

void CoinLayer::OnRender() {
   Vector2 coinTextureSize = {(float)m_coinTexture.width, (float)m_coinTexture.height};
   Vector2 coinAmountSize = MeasureTextEx(App::font_semibold, amount.first.c_str(), amount.second, 1.5f);
   Vector2 padding = {10, 10};

   Vector2 boxSize = coinAmountSize + coinTextureSize + padding*2 + Vector2{padding.x * 1.5f, 0}; // extra 10 for padding between coin and amount
   boxSize.y = std::max(coinAmountSize.y, coinTextureSize.y) + padding.y;
   Rectangle coinBox = {((float)GetScreenWidth() - boxSize.x)/2, 20, boxSize.x, boxSize.y};

   DrawRectangleRounded(coinBox, 5.0f, 5, WHITE);
   DrawTexture(m_coinTexture, coinBox.x + padding.x + 2, coinBox.y + padding.y - 1, WHITE); // ±2 for centering texture
   DrawTextEx(
      App::font_semibold, amount.first.c_str(),
      {coinBox.x + padding.x + coinTextureSize.x + padding.x, coinBox.y + (coinBox.height - coinAmountSize.y) / 2},
      amount.second, 1.5f, ColorAlpha(DARKERGRAY, 0.98f)
   );
}

void CoinLayer::resize() {
   amount.second = std::clamp(GetScreenHeight() / 23.809f, 20.0f, 40.0f);
   TraceLog(LOG_INFO, "RESIZE: Coin font resized to: %d", amount.second);

   float scale = std::clamp(GetScreenHeight() / 650.0f, 1.0f, 2.0f);
   ImageResize(&m_coinImage, 20 * scale, 20 * scale); // original coin texture size is 20 x 20
   UnloadTexture(m_coinTexture);
   m_coinTexture = LoadTextureFromImage(m_coinImage);
   TraceLog(LOG_INFO, "RESIZE: Coin icon resize to: %f, %f", 20 * scale, 20 * scale);
}