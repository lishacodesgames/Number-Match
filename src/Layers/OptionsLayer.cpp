#include <pch/Precompiled.h>
#include "OptionsLayer.h"

#include "HomeLayer.h"
#include "Core/Event.h"
#include "Colors.h"
#include "App.h"

static constexpr int PANEL_HEIGHT = 35;
static constexpr float BANNER_HEIGHT = 32.0f;

static constexpr float BOUNDS_TARGETY = 100.0f;

OptionsLayer::OptionsLayer() : Core::Layer("Options Layer", true),
      m_bounds({
         (float)(GetScreenWidth()) / 4, (float)(GetScreenHeight()), // to be animated to target height
         (float)(GetScreenWidth()) / 2, (float)(GetScreenHeight()) / 2 + 100
      }),
      m_doneButton(
         {m_bounds.x + m_bounds.width - 55, m_bounds.y + 7}, // origin 
         {0, 0}, "Done", BLANK, BLUE, 20, {0, 0}, App::font_semibold
      )
{
   m_rightArrowTexture = LoadTexture("assets/icons/options/rightarrow_10x13.png");

   // banners
   setBannerPositions(m_bounds.y);

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

void OptionsLayer::OnEvent(Core::Event& e) {
   e.Handled = true; // don't want any events to pass through to gamelayer
   if(e.GetEventType() == Core::EventType::MouseClicked && m_doneButton.isHovered) {
      App::QueueLayerPop(this);

      // we're sure that game exists bc OptionsLayer only exists in its context
      App::GetLayerByName("Game Layer")->OnResume();
      e.Handled = true;
   } else if(e.GetEventType() == Core::EventType::KeyPressed) {
      char key = static_cast<Core::KeyPressedEvent&>(e).key;
      if(key == 'q' || key == 'Q') {
         App::GetLayerByName("Game Layer")->OnSuspend();
         App::QueueLayerSwap(this, new HomeLayer());
         e.Handled = true;
      }
   }
}

void OptionsLayer::OnUpdate() {
   if(IsWindowResized()) {
      setPanel();
      setBannerPositions(m_bounds.y);
   }

   if(m_bounds.y > BOUNDS_TARGETY) {
      double dt = std::min(GetFrameTime(), 0.033f); // for a delay of atleast 33ms
      double speed = 160;
      double diff = m_bounds.y - BOUNDS_TARGETY;

      m_bounds.y -= 0.1f * diff * speed * dt; // move 10% of remaining distance with delay and lag buffer
      if(diff < 1.5) // within 1.5 pixels
         m_bounds.y = BOUNDS_TARGETY; // snap to target, since %-based approach is an asymptote
      
      setBannerPositions(m_bounds.y);
      m_doneButton.setOrigin({m_doneButton.getOrigin().x, m_bounds.y + 7});
      return;
   }

   m_doneButton.Update(); 

   if(m_doneButton.isHovered)
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);

   for(Rectangle banner : m_banners)
      if(CheckCollisionPointRec(GetMousePosition(), banner))
         SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
}

void OptionsLayer::OnRender() {
   DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {80, 80, 80, 180}); // to make the bg darker
   DrawRectangleRounded(m_bounds, 0.1f, 6, LIGHTERGRAY);  // main popup

   renderTopPanel();
   renderBlankBanners();
   renderBannerContent();
}

#pragma region Helpers

void OptionsLayer::setPanel() {
   m_bounds = {
      (float)GetScreenWidth() * 0.25f, BOUNDS_TARGETY,
      (float)GetScreenWidth() * 0.5f, (float)GetScreenHeight() * 0.5f + 100
   };

   m_doneButton.setOrigin({m_bounds.x + m_bounds.width - 55, m_bounds.y + 7});
}

void OptionsLayer::setBannerPositions(float boundsY) {
   float originX = m_bounds.x + m_bounds.width * 0.16f / 2; // half of 16%
   float originY = boundsY + PANEL_HEIGHT * 1.5f;
   Vector2 size = {m_bounds.width * 0.85f, BANNER_HEIGHT}; // 85% of popup

   constexpr float spacing = BANNER_HEIGHT + 23; // Space between the top and 2 bottom banners (ref: settings.jpg)

   m_banners[SETTINGS] = {originX, originY, size.x, size.y};

   m_banners[HOW_TO] = {originX, originY + spacing, size.x, size.y};
   m_banners[HELP] = {originX, originY + spacing + BANNER_HEIGHT, size.x, size.y};
   m_banners[ABOUT] = {originX, originY + spacing + BANNER_HEIGHT * 2, size.x, size.y};
   m_banners[PRIVACY] = {originX, originY + spacing + BANNER_HEIGHT * 3, size.x, size.y};
   m_banners[PREFS] = {originX, originY + spacing + BANNER_HEIGHT * 4, size.x, size.y};

   m_banners[MATH] = {originX, originY + spacing * 2 + BANNER_HEIGHT * 4, size.x, size.y};
   m_banners[NO_ADS] = {originX, originY + spacing * 3 + BANNER_HEIGHT * 4, size.x, size.y};
}

void OptionsLayer::renderTopPanel() {
   Rectangle panel = m_bounds;
   panel.height = PANEL_HEIGHT;

   Rectangle panelSharpBottom = panel;
   panelSharpBottom.y += PANEL_HEIGHT / 2;
   panelSharpBottom.height = PANEL_HEIGHT / 2;

   DrawRectangleRounded(panel, 0.8f, 6, WHITE);
   DrawRectangleRec(panelSharpBottom, WHITE);
   DrawTextEx(
      App::font_semibold, "Options", 
      {m_bounds.x + m_bounds.width / 2 - 30, m_bounds.y + 7}, 
      20, 1, BLACK
   );

   m_doneButton.Draw();
}

void OptionsLayer::renderBlankBanners() {
   // base banner shape
   for(Rectangle banner : m_banners)
      DrawRectangleRounded(banner, 0.5f, 4, WHITE);

   // sharp rectangle to cover touching rounded corners
   Rectangle secondBanner = m_banners.at(HOW_TO);
   DrawRectangleV(
      {secondBanner.x, secondBanner.y + secondBanner.height/2},
      {secondBanner.width, BANNER_HEIGHT*4}, // 5 rectangles but cutting out half a rect from top and bottom
      WHITE
   );

   // lines between big block
   for(int i = HOW_TO; i < PREFS; i++)
      DrawLine(
         m_banners.at(i).x, 
         m_banners.at(i).y + m_banners.at(i).height,
         m_banners.at(i).x + m_banners.at(i).width, 
         m_banners.at(i).y + m_banners.at(i).height,
         LIGHTERGRAY
      );

   // right arrow icon on all except last 2
   for(int i = SETTINGS; i <= PREFS; i++)
      DrawTexture(
         m_rightArrowTexture, 
         m_banners.at(i).x + m_banners.at(i).width - 10*2,
         m_banners.at(i).y + 10, 
         LIGHTGRAY
      );
}

void OptionsLayer::renderBannerContent() {
   Texture icon;
   Rectangle banner;
   std::string name;
   float padding;

   for(int i = SETTINGS; i <= NO_ADS; i++) {
      icon = m_bannerIcons.at(i);
      banner = m_banners.at(i);
      name = m_bannerNames.at(i);
      padding = (BANNER_HEIGHT - icon.height)/2;

      Vector2 iconPos = {banner.x + padding * 1.5f, banner.y + padding};
      DrawTextureEx(icon, iconPos, 0, 0.98f, WHITE); // new scaled height = 23.52
      DrawTextEx(
         App::font_semibold, name.c_str(),
         {iconPos.x + icon.width + 15, iconPos.y + 2},
         20, 1, DARKGRAY
      );
   }
}

#pragma endregion