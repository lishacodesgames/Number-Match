#include <pch/Precompiled.h>
#include "CoinLayer.h"

#include "PanelLayer.h"
#include "Storage.h"
#include "Colors.h"
#include "App.h"

Rectangle CoinLayer::box = { 0, 0, 0, 0 };

CoinLayer::CoinLayer() : Core::Layer("Coin Layer", true) {
   m_coinImage = LoadImage("assets/icons/game/coin_20x20.png");
   m_coinTexture = LoadTextureFromImage(m_coinImage);
   resize();
}

void CoinLayer::OnUpdate() {
   // cannot be in OnEvent since CoinLayer is sometimes on top of PanelLayer so the coin's check happens before layer is switched
   // causing coins to not update when switching from Home to Daily/Me, since PanelLayer is popped after the click event is processed
   // leads to buggy mess lol
   if(App::GetLayerByName("Panel Layer") && !App::GetLayerByName("Home Layer"))
      App::QueueLayerPop(this);

   if(IsWindowResized())
      resize();
}

void CoinLayer::OnRender() {
   Vector2 coinAmountSize = MeasureTextEx(App::font_semibold, Storage::formatCoins().c_str(), m_fontSize, 1.5f);

   int rs = 5; // Roundsness & Segments
   DrawRectangleRounded(box, rs, rs, BRIGHT_BG);
   DrawRectangleRoundedLinesEx(box, rs, rs, 2, SHADOW_FOR_BRIGHT);

   DrawTexture(m_coinTexture, box.x + m_coinTexture.width * 0.3f, box.y + (box.height - m_coinTexture.height) / 2, WHITE);
   DrawTextEx(
      App::font_semibold, Storage::formatCoins().c_str(),
      {  box.x + m_coinTexture.width * 1.5f,
         box.y + (box.height - coinAmountSize.y) / 2 },
      m_fontSize, 1.5f, TEXT_FOR_BRIGHT
   );
}

void CoinLayer::resize() {
   float old = m_fontSize;
   m_fontSize = std::max(GetScreenHeight() / 23.809f, 20.0f);
   if(std::abs(m_fontSize - old) > 0.5f)  // to avoid polluting the terminal with unnecssary logs
      LOG_RESIZE("Coin font resized to: %f", m_fontSize);

   float scale = std::clamp(GetScreenHeight() / 650.0f, 1.0f, 2.5f);
   ImageResize(&m_coinImage, 20 * scale, 20 * scale);  // original coin texture size is 20 x 20
   if(m_coinTexture.height != m_coinImage.height) {
      UnloadTexture(m_coinTexture);
      m_coinTexture = LoadTextureFromImage(m_coinImage);
      LOG_RESIZE("Coin icon resized to: %d, %d", m_coinTexture.width, m_coinTexture.height);
   }

   std::string coinAmount = Storage::formatCoins();
   Vector2 coinAmtSize = MeasureTextEx(App::font_semibold, coinAmount.c_str(), m_fontSize, 1.5f);
   float boxWidth = coinAmtSize.x + m_coinTexture.width * 1.9f;
   float boxHeight = std::max(coinAmtSize.y, (float)m_coinTexture.height) + 10;
   box = { (GetScreenWidth() - boxWidth) / 2.0f, boxHeight / 5.0f, boxWidth, boxHeight };
}