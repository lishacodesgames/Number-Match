#include <pch/Precompiled.h>
#include "OptionsLayer.h"

#include "HomeLayer.h"
#include "Core/Event.h"
#include "Colors.h"
#include "App.h"

static constexpr float PANEL_PROPORTION = 0.085f;
static constexpr float BANNER_PROPORTION = 0.075f;

OptionsLayer::OptionsLayer() : Core::Layer("Options Layer", true),
      m_doneButton({ 0, 0 }, { 2, 2 }, "Done", BLANK, Palette::game_nav_color, 20, { 0, 0 }, App::font_semibold),
      m_gobackButton({ 0, 0 }, { 2, 2 }, "", BLANK, Palette::game_nav_color)
{
   m_rightArrowTexture = LoadTexture("assets/icons/options/rightarrow_10x13.png");
   m_gobackButton.setIcon("assets/icons/game/goback_18x24.png");
   setBounds(GetScreenHeight());

   // banners
   setBannerPositions();

   // banner icons
   m_bannerIcons[SETTINGS] = LoadTexture("assets/icons/options/settings_24x24.png");
   m_bannerIcons[HOW_TO] = LoadTexture("assets/icons/options/howto_24x24.png");
   m_bannerIcons[HELP] = LoadTexture("assets/icons/options/help_24x24.png");
   m_bannerIcons[ABOUT] = LoadTexture("assets/icons/options/about_24x24.png");
   m_bannerIcons[PRIVACY] = LoadTexture("assets/icons/options/privacy_24x24.png");
   m_bannerIcons[PREFS] = LoadTexture("assets/icons/options/prefs_24x24.png");
   m_bannerIcons[MATH] = LoadTexture("assets/icons/options/math_24x24.png");
   m_bannerIcons[NO_ADS] = LoadTexture("assets/icons/options/noads_24x24.png");

   // verification
   for(int i = SETTINGS; i <= NO_ADS; i++)
      if(!IsTextureValid(m_bannerIcons.at(i)))
         TraceLog(LOG_ERROR, "Banner icon at index %d failed to load!", i);

   // banner names, no map bcz i wanna use indices in iteration
   m_bannerNames[SETTINGS] = "Settings";
   m_bannerNames[HOW_TO] = "How To Play";
   m_bannerNames[HELP] = "Help";
   m_bannerNames[ABOUT] = "About Game";
   m_bannerNames[PRIVACY] = "Privacy Rights";
   m_bannerNames[PREFS] = "Privacy Preferences";
   m_bannerNames[MATH] = "Math Puzzle";
   m_bannerNames[NO_ADS] = "Remove Ads";
}

OptionsLayer::~OptionsLayer() {
   for(Texture& icon : m_bannerIcons)
      UnloadTexture(icon);
}

void OptionsLayer::OnEvent(Core::Event& e) {
   e.Handled = true;  // don't want any events to pass through to gamelayer
   if(e.GetEventType() == Core::EventType::MouseClicked) {
      if(m_doneButton.isHovered) {
         App::QueueLayerPop(this);

         // we're sure that game exists bc OptionsLayer only exists in its context
         App::GetLayerByName("Game Layer")->OnResume();
         e.Handled = true;
      } else if(CheckCollisionPointRec(GetMousePosition(), m_banners.at(SETTINGS))) {
         currentPage = Page::Settings;
         e.Handled = true;
      } else if(m_gobackButton.isHovered) {
         currentPage = Page::Options;
      }
   } else if(e.GetEventType() == Core::EventType::KeyPressed) {
      char key = static_cast<Core::KeyPressedEvent&>(e).key;

      /// @bug if we quit gamelayer from optionslayer then try to go back to game, game crashes
      if(key == 'q' || key == 'Q') {
         if(currentPage == Page::Options) {
            App::GetLayerByName("Game Layer")->OnSuspend();
            App::QueueLayerSwap(this, new HomeLayer());
            e.Handled = true;
         } else
            currentPage = Page::Options;
      }
   }
}

void OptionsLayer::OnUpdate() {
   if(IsWindowResized()) {
      setBounds(m_targetY);
      setBannerPositions();
   }

   if(m_bounds.y > m_targetY) {
      double dt = std::min(GetFrameTime(), 0.033f);
      double speed = 160;
      double diff = m_bounds.y - m_targetY;

      m_bounds.y -= 0.1f * diff * speed * dt;  // move 10% of remaining distance with delay and lag buffer
      if(diff < 1.5)                           // within 1.5 pixels
         m_bounds.y = m_targetY;               // snap to target, since %-based approach is an asymptote

      setBounds(m_bounds.y);   
      setBannerPositions();
      return;
   }

   m_doneButton.Update();
   if(currentPage == Page::Settings)
      m_gobackButton.Update();

   if(m_doneButton.isHovered || m_gobackButton.isHovered || getHoveredBannerIndex() != -1)
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
         panel.y + (panel.height - titleSize.y) / 2},
      titleFontSize, 1.2f, Palette::options_title_color
   );

   m_doneButton.Draw();

   if(currentPage == Page::Options) {
      renderBlankBanners();
      renderBannerContent();
   } else if(currentPage == Page::Settings) {
      DrawTextEx(GetFontDefault(), "Hello World!", { m_bounds.x + 20, m_bounds.y + 50 }, 20, 1, PURPLE);
      m_gobackButton.Draw();
   }
}

#pragma region Helpers

void OptionsLayer::setBounds(float targetY) {
   m_targetY = GetScreenHeight() / 6;

   // max aspect ratio of 4 : 3
   float aspect = std::min((float)GetScreenWidth() / GetScreenHeight(), 4.0f / 3.0f);
   float height = GetScreenHeight() * 0.7f;
   float width = height * aspect;
   m_bounds = {
      (GetScreenWidth() - width) / 2, targetY,
      width, height
   };
   
   // panel buttons
   const float panelHeight = m_bounds.height * PANEL_PROPORTION;

   m_doneButton.setFontSize((int)(panelHeight / 2));
   m_doneButton.setOrigin({
      m_bounds.x + m_bounds.width - m_doneButton.getSize().x * 1.3f,
      m_bounds.y + (panelHeight - m_doneButton.getSize().y) / 2
   });

   m_gobackButton.setFontSize(m_doneButton.getFontSize());
   m_gobackButton.setOrigin({
      m_bounds.x + m_gobackButton.getSize().x * 0.75f,
      m_bounds.y + (panelHeight - m_gobackButton.getSize().y) / 2
   });
}

void OptionsLayer::setBannerPositions() {
   float originX = m_bounds.x + m_bounds.width * 0.16f / 2;  // half of 16%
   float originY = m_bounds.y + m_bounds.height * PANEL_PROPORTION * 1.5f;
   Vector2 size = { m_bounds.width * 0.85f, m_bounds.height * BANNER_PROPORTION };  // 85% of popup

   const float spacing = m_bounds.height * BANNER_PROPORTION + 23;  // Space between the top and 2 bottom banners (ref: settings.jpg)

   m_banners[SETTINGS] = { originX, originY, size.x, size.y };

   m_banners[HOW_TO] = { originX, originY + spacing, size.x, size.y };
   m_banners[HELP] = { originX, originY + spacing + m_bounds.height * BANNER_PROPORTION, size.x, size.y };
   m_banners[ABOUT] = { originX, originY + spacing + m_bounds.height * BANNER_PROPORTION * 2, size.x, size.y };
   m_banners[PRIVACY] = { originX, originY + spacing + m_bounds.height * BANNER_PROPORTION * 3, size.x, size.y };
   m_banners[PREFS] = { originX, originY + spacing + m_bounds.height * BANNER_PROPORTION * 4, size.x, size.y };

   m_banners[MATH] = { originX, originY + spacing * 2 + m_bounds.height * BANNER_PROPORTION * 4, size.x, size.y };
   m_banners[NO_ADS] = { originX, originY + spacing * 3 + m_bounds.height * BANNER_PROPORTION * 4, size.x, size.y };
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
         Palette::options_arrow_color
      );
   }
}

void OptionsLayer::renderBannerContent() const {
   for(int i = SETTINGS; i <= NO_ADS; i++) {
      const Texture& icon = m_bannerIcons.at(i);
      const Rectangle& banner = m_banners.at(i);
      const std::string& name = m_bannerNames.at(i);
      float padding = (m_bounds.height * BANNER_PROPORTION - icon.height) / 2;

      Vector2 iconPos = { banner.x + padding * 1.5f, banner.y + padding };
      DrawTextureEx(icon, iconPos, 0, 0.98f, WHITE);  // new scaled height = 23.52
      DrawTextEx(
         App::font_semibold, name.c_str(),
         { iconPos.x + icon.width + 15, iconPos.y + 2 },
         20, 1, Palette::options_text_color
      );
   }
}

int OptionsLayer::getHoveredBannerIndex() const {
   for(size_t i = 0; i < m_banners.size(); i++)
      if(CheckCollisionPointRec(GetMousePosition(), m_banners.at(i)))
         return (int)i;
   return -1;
}

#pragma endregion