#include <pch/Precompiled.h>
#include "CoinLayer.h"

#include "PanelLayer.h"
#include "Storage.h"
#include "App.h"

void CoinLayer::OnUpdate() {
   // cannot be in OnEvent since CoinLayer is sometimes on top of PanelLayer so the coin's check happens before layer is switched
   // causing coins to not update when switching from Home to Daily/Me, since PanelLayer is popped after the click event is processed
   // leads to buggy mess lol
   if(PanelLayer::currentPage == Menu::Daily || PanelLayer::currentPage == Menu::Me) 
      App::QueueLayerPop(this);
}

void CoinLayer::OnRender() {
   std::string coins = Storage::formatCoins();
   float fontSize = 27.3f;
   float spacing = 1.5f;
   
   Vector2 coinTextureSize = {(float)m_coinTexture.width, (float)m_coinTexture.height};
   Vector2 coinAmountSize = MeasureTextEx(App::font_semibold, coins.c_str(), fontSize, spacing);
   Vector2 padding = {10, 10};

   Vector2 boxSize = coinAmountSize + coinTextureSize + padding*2 + Vector2{10, 0}; // extra 10 for padding between coin and amount
   boxSize.y = std::max(coinAmountSize.y, coinTextureSize.y) + padding.y;
   Rectangle coinBox = {((float)GetScreenWidth() - boxSize.x)/2, 20, boxSize.x, boxSize.y};

   DrawRectangleRounded(coinBox, 5.0f, 5, WHITE);
   DrawTexture(m_coinTexture, coinBox.x + padding.x + 2, coinBox.y + padding.y - 2, WHITE); // ±2 for centering texture
   DrawTextEx(
      App::font_semibold, coins.c_str(),
      {coinBox.x + padding.x + coinTextureSize.x + padding.x, coinBox.y + 5.85f},
      fontSize, spacing, {0, 0, 0, 175}
   );
}
