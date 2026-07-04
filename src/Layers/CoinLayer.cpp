#include <pch/Precompiled.h>
#include "CoinLayer.h"

#include "Utils/Numbers.h"
#include "Storage.h"
#include "Colors.h"
#include "App.h"

Rectangle CoinLayer::box = { 0, 0, 0, 0 };

CoinLayer::CoinLayer() : Core::Layer("Coin Layer", true),
      m_coinTexture(LoadTexture("assets/icons/game/coin_20x20.png"))
{ resize(); }

void CoinLayer::OnEvent(Core::Event& e) {
   if(e.GetEventType() == Core::EventType::WindowResize)
      resize();
}

void CoinLayer::OnUpdate() {}

void CoinLayer::OnRender() {
   int rs = 5; // Roundsness & Segments
   DrawRectangleRounded(box, rs, rs, Palette::bright_bg);
   DrawRectangleRoundedLinesEx(box, rs, rs, 2, Palette::shadow_for_bright);
   
   float texDim = 20 * m_textureScale;
   DrawTextureEx(m_coinTexture,
      {  box.x + texDim * 0.3f,
         box.y + (box.height - texDim) / 2 },
      0.0f, m_textureScale, WHITE);

   Vector2 coinAmountSize =
      MeasureTextEx(App::font_semibold, Utils::formatNumber(Storage::game.coins).c_str(), m_fontSize, 1.5f);
   DrawTextEx(
      App::font_semibold, Utils::formatNumber(Storage::game.coins).c_str(),
      {  box.x + texDim * 1.5f,
         box.y + (box.height - coinAmountSize.y) / 2 },
      m_fontSize, 1.5f, Palette::text_for_bright);
}

void CoinLayer::resize() {
   float old = m_fontSize;
   m_fontSize = std::max(GetScreenHeight() / 23.809f, 20.0f);
   if(std::abs(m_fontSize - old) > 0.5f)  // to avoid polluting the terminal with unnecssary logs
      LOG_RESIZE("Coin font -> {}", m_fontSize);

   old = m_textureScale;
   m_textureScale = std::clamp(GetScreenHeight() / 650.0f, 1.0f, 2.5f);
   float texDim = 20.0f * m_textureScale;
   if(std::abs(m_textureScale - old) > 0.5f)
      LOG_RESIZE("Coin icon -> {} x {}", texDim, texDim);

   Vector2 coinAmtSize = MeasureTextEx(App::font_semibold, Utils::formatNumber(Storage::game.coins).c_str(), m_fontSize, 1.5f);
   float boxWidth = coinAmtSize.x + texDim * 1.9f;
   float boxHeight = std::max(coinAmtSize.y, texDim) + 10;
   box = { (GetScreenWidth() - boxWidth) / 2.0f, boxHeight / 5.0f, boxWidth, boxHeight };
}