#pragma once
#include "Core/Event.h"
#include "Core/Layer.h"
#include "GUI/Button.h"
#include "GUI/Toggle.h"

#include <raylib.h>
#include <string>
#include <array>

class OptionsLayer : public Core::Layer {
public:
   OptionsLayer();
   ~OptionsLayer() override;

   void OnAttach() override {
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
      Core::Layer::OnAttach();
   }

   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;

private:
   Texture m_rightArrowTexture = { 0 };

   float m_targetY = 0.0f;
   Rectangle m_bounds = {};

   GUI::Button m_doneButton;

   std::array<Rectangle, 8> m_banners;
   std::array<Texture, 8> m_bannerIcons;
   std::array<std::string, 8> m_bannerNames;

   /// simply for more readability when looping over parts of the banners' arrays
   enum BannerTypes { SETTINGS, HOW_TO, HELP, ABOUT, PRIVACY, PREFS, MATH, NO_ADS }; 
   enum class Page { Options, Settings }; // more later

   Page currentPage = Page::Options;
private:
   // per page members
   GUI::Button m_gobackButton;
   GUI::Toggle m_darkModeToggle;
private:
   void resize(float targetY);  /// @param init whether to apply targetY or not (for animation)
   void setBannerPositions();

   void renderBlankBanners() const;
   void renderBannerContent() const;

   int getHoveredBannerIndex() const;
};