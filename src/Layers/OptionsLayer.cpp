#include <pch/Precompiled.h>
#include "OptionsLayer.h"

#include "GUI/Animation.h"
#include "Utils/Numbers.h"
#include "PanelLayer.h"
#include "HomeLayer.h"
#include "Storage.h"
#include "Colors.h"
#include "App.h"

static constexpr float PANEL_PROPORTION = 0.085f;
static constexpr float BANNER_PROPORTION = 0.075f;

OptionsLayer::OptionsLayer() : Core::Layer("Options Layer", true),
      m_doneButton(
         { 0, 0 }, { 2, 2 }, "Done", BLANK, Palette::game_nav_color, 20, { 0, 0 }, App::font_semibold),
      m_gobackButton({ 0, 0 }, { 2, 2 }, "", BLANK, Palette::game_nav_color),
      m_rightArrowTexture(LoadTexture("assets/icons/options/rightarrow_10x13.png")),
      m_bannerIcons({
         LoadTexture("assets/icons/options/settings_24x24.png"),
         LoadTexture("assets/icons/options/howto_24x24.png"),
         LoadTexture("assets/icons/options/help_24x24.png"),
         LoadTexture("assets/icons/options/about_24x24.png"),
         LoadTexture("assets/icons/options/privacy_24x24.png"),
         LoadTexture("assets/icons/options/prefs_24x24.png"),
         LoadTexture("assets/icons/options/math_24x24.png"),
         LoadTexture("assets/icons/options/noads_24x24.png") }),
      m_bannerNames({
         "Settings",
         "How To Play",
         "Help",
         "About Game",
         "Privacy Rights",
         "Privacy Preferences",
         "Math Puzzles",
         "Remove Ads" })
{
   m_gobackButton.setIcon("assets/icons/game/goback_18x24.png");

   m_themeToggle.bgColor = Palette::off_bright_bg;
   m_themeToggle.knobColor = Palette::shadow_for_off_bright;
   m_themeToggle.setState(Storage::ui.isDarkMode);

   resize(GetScreenHeight());
}

void OptionsLayer::OnEvent(Core::Event& e) {
   e.Handled = true;  // don't want any events to pass through to gamelayer
   if(e.GetEventType() == Core::EventType::MouseClicked) {
      if(m_doneButton.isHovered) {
         App::QueueLayerPop(this);

         // we're sure that game exists bc OptionsLayer only exists in its context
         App::GetLayerByName("Game Layer")->OnResume();
         e.Handled = true;
         return;
      } else if(CheckCollisionPointRec(GetMousePosition(), m_banners.at(SETTINGS))) {
         currentPage = Page::Settings;
         e.Handled = true;
         return;
      } else if(m_gobackButton.isHovered) {
         currentPage = Page::Options;
         e.Handled = true;
         return;
      } else if(m_themeToggle.isHovered) {
         if(Storage::ui.isDarkMode)
            Palette::SetLightMode();
         else
            Palette::SetDarkMode();

         /// @todo fix. Shouldn't have to do this every time
         m_themeToggle.bgColor = Palette::off_bright_bg;
         m_themeToggle.knobColor = Palette::shadow_for_off_bright;
         e.Handled = true;
         return;
      }
   } else if(e.GetEventType() == Core::EventType::KeyPressed) {
      char key = static_cast<Core::KeyPressedEvent&>(e).key;
      if(key == 'q' || key == 'Q') {
         if(currentPage == Page::Options) {
            App::GetLayerByName("Game Layer")->OnSuspend();
            App::QueueLayerSwap(this, new HomeLayer());
            App::QueueLayerPush(new PanelLayer());
         } else
            currentPage = Page::Options;
         e.Handled = true;
      }
   } else if(e.GetEventType() == Core::EventType::WindowResize)
      resize(m_targetY);
}

void OptionsLayer::OnUpdate() {
   if(m_bounds.y > m_targetY) {
      resize(GUI::LERP(m_bounds.y, m_targetY, 160.0f, 2.0f));   
      return;
   }

   m_doneButton.Update();
   if(currentPage == Page::Settings) {
      m_gobackButton.Update();
      m_themeToggle.Update();
   }

   bool ismouseHovering =
         m_doneButton.isHovered 
         || (currentPage == Page::Options && getHoveredBannerIndex() != -1)
         || (currentPage == Page::Settings && (m_gobackButton.isHovered || m_themeToggle.isHovered));

   if(ismouseHovering)
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void OptionsLayer::OnRender() {
   DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Palette::darken_game_bg);
   DrawRectangleRounded(m_bounds, 0.1f, 6, Palette::options_bg); // main popup

   Rectangle panel = m_bounds;
   panel.height *= PANEL_PROPORTION;

   Rectangle panelSharpBottom = panel;
   panelSharpBottom.y += panel.height / 2;
   panelSharpBottom.height = panel.height / 2;

   DrawRectangleRounded(panel, 0.8f, 6, Palette::bright_bg);
   DrawRectangleRec(panelSharpBottom, Palette::bright_bg);

   float titleFontSize = m_doneButton.getFontSize() * 1.3f;
   Vector2 titleSize = MeasureTextEx(App::font_semibold, "Options", titleFontSize, 1);
   DrawTextEx(
      App::font_semibold, "Options",
      {  panel.x + (panel.width - titleSize.x) / 2,
         panel.y + (panel.height - titleSize.y) / 2 },
      titleFontSize, 1.2f, Palette::options_title_color);

   m_doneButton.Draw();

   if(currentPage == Page::Options) {
      renderBlankBanners();
      renderBannerContent();
   } else if(currentPage == Page::Settings) {
      m_gobackButton.Draw();
      m_themeToggle.Draw();
   }
}

#pragma region Helpers

/// @bug fix fontSize resizing
void OptionsLayer::resize(float boundsY) {
   // panel
   m_targetY = GetScreenHeight() / 6.0f;
   Vector2 oldSize = { m_bounds.width, m_bounds.height };

   // max aspect ratio of 4 : 3
   float aspect = std::min((float)GetScreenWidth() / GetScreenHeight(), 4.0f / 3.0f);
   float height = GetScreenHeight() * 0.7f;
   float width = height * aspect;
   m_bounds = { (GetScreenWidth() - width) / 2, boundsY, width, height };

   if(Utils::avgAbsDiff(oldSize, { m_bounds.width, m_bounds.height }) >= 0.5f)
      LOG_RESIZE("Options panel -> {} x {}", m_bounds.width, m_bounds.height);

   // panel buttons
   const float panelHeight = m_bounds.height * PANEL_PROPORTION;

   m_doneButton.setFontSize((panelHeight / 2));
   m_doneButton.setOrigin({
      m_bounds.x + m_bounds.width - m_doneButton.getSize().x * 1.3f,
      m_bounds.y + (panelHeight - m_doneButton.getSize().y) / 2.0f });

   m_gobackButton.setFontSize(m_doneButton.getFontSize());
   m_gobackButton.setOrigin({
      m_bounds.x + m_gobackButton.getSize().x * 0.75f,
      m_bounds.y + (panelHeight - m_gobackButton.getSize().y) / 2.0f });

   // theme toggle
   Vector2 newToggleSize, oldToggleSize = m_themeToggle.getSize();
   newToggleSize.x = std::max(m_bounds.width / 2.5f, 200.0f);
   newToggleSize.y = std::min(m_bounds.height / 4.0f, newToggleSize.x / 2.1f);
   m_themeToggle.setSize(newToggleSize);
   m_themeToggle.setPadding(std::min(newToggleSize.x, newToggleSize.y) * 0.075f);

   if(Utils::avgAbsDiff(oldToggleSize, newToggleSize) >= 0.5f)
      LOG_RESIZE("Dark mode toggle -> {} x {}", newToggleSize.x, newToggleSize.y);

   m_themeToggle.setOrigin({
      m_bounds.x + (m_bounds.width - newToggleSize.x) / 2.0f,
      m_bounds.y + (m_bounds.height - newToggleSize.y) / 2.0f });

   // banners
   float originX = m_bounds.x + m_bounds.width * 0.16f / 2;  // half of 16%
   float originY = m_bounds.y + panelHeight * 1.5f;
   float bannerHeight = m_bounds.height * BANNER_PROPORTION;
   Vector2 size = { m_bounds.width * 0.85f, bannerHeight };  // 85% of popup

   const float spacing = bannerHeight * 1.75f;  // Space between the top and 2 bottom banners (ref: settings.jpg)

   m_banners[SETTINGS] = { originX, originY, size.x, size.y };

   m_banners[HOW_TO] = { originX, originY + spacing, size.x, size.y };
   m_banners[HELP] = { originX, originY + spacing + bannerHeight, size.x, size.y };
   m_banners[ABOUT] = { originX, originY + spacing + bannerHeight * 2, size.x, size.y };
   m_banners[PRIVACY] = { originX, originY + spacing + bannerHeight * 3, size.x, size.y };
   m_banners[PREFS] = { originX, originY + spacing + bannerHeight * 4, size.x, size.y };

   m_banners[MATH] = { originX, originY + spacing * 2 + bannerHeight * 4, size.x, size.y };
   m_banners[NO_ADS] = { originX, originY + spacing * 3 + bannerHeight * 4, size.x, size.y };
}

void OptionsLayer::renderBlankBanners() const {
   int hovered = getHoveredBannerIndex();

   // base banner shape
   for(size_t i = 0; i < m_banners.size(); i++) {
      const Rectangle& banner = m_banners.at(i);
      Color bg = ((int)i == hovered ? Palette::shadow_for_bright : Palette::bright_bg);

      if(i == SETTINGS || i == MATH || i == NO_ADS)   // round banners
         DrawRectangleRounded(banner, 0.5f, 4, bg);
      else if(i == HOW_TO || i == PREFS) {            // partially round banners
         Rectangle sharp = { banner.x, banner.y, banner.width, banner.height / 2 }; // top half sharp rectangle
         if(i == HOW_TO) 
            sharp.y += sharp.height;                                                // bottom half-ify
         
         DrawRectangleRounded(banner, 0.5f, 4, bg);
         DrawRectangleRec(sharp, bg);
      }
      else                                            // not-round-at-all banners
         DrawRectangleRec(banner, bg);
   }

   // lines for middle banner block
   for(int i = HOW_TO; i < PREFS; i++) {
      const Rectangle& banner = m_banners.at(i + 1);
      DrawLineEx({ banner.x, banner.y }, { banner.x + banner.width, banner.y }, 1, Palette::shadow_for_bright);
   }

   // right arrow icon on all except last 2
   for(int i = SETTINGS; i <= PREFS; i++) {
      const Rectangle& banner = m_banners.at(i);
      DrawTexture(
         m_rightArrowTexture,
         banner.x + banner.width - m_rightArrowTexture.width * 1.7f,
         banner.y + (banner.height - m_rightArrowTexture.height) / 2.0f,
         Palette::options_arrow_color);
   }
}

void OptionsLayer::renderBannerContent() const {
   for(int i = SETTINGS; i <= NO_ADS; i++) {
      const Texture& icon = m_bannerIcons.at(i);
      const Rectangle& banner = m_banners.at(i);
      const std::string& name = m_bannerNames.at(i);
      float padding = (m_bounds.height * BANNER_PROPORTION - icon.height) / 2.0f;

      Vector2 iconPos = { banner.x + padding * 1.5f, banner.y + padding };
      DrawTextureEx(icon, iconPos, 0, 0.98f, WHITE);  // new scaled height = 23.52
      DrawTextEx(
         App::font_semibold, name.c_str(),
         { iconPos.x + icon.width + 15, iconPos.y + 2 },
         20, 1, Palette::options_text_color);
   }
}

int OptionsLayer::getHoveredBannerIndex() const {
   for(int i = 0; i < (int)m_banners.size(); i++)
      if(CheckCollisionPointRec(GetMousePosition(), m_banners.at(i)))
         return i;

   return -1;
}

#pragma endregion