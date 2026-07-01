#pragma once
#include "Core/Layer.h"
#include "GUI/Button.h"
#include "GUI/Toggle.h"

#include <raylib.h>
#include <string>
#include <array>

class OptionsLayer : public Core::Layer {
public:
   OptionsLayer();
   ~OptionsLayer() override {
      UnloadTexture(m_rightArrowTexture);
      for(const Texture& icon : m_bannerIcons)
         UnloadTexture(icon);
   }

   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;

private:
   /// simply for more readability when looping over parts of the banners' arrays
   enum BannerTypes { SETTINGS, HOW_TO, HELP, ABOUT, PRIVACY, PREFS, MATH, NO_ADS }; 
   enum class Page { Options, Settings }; // more later

private:
   float m_targetY;
   Rectangle m_bounds;

   GUI::Button m_doneButton;
   Page currentPage = Page::Options;

private:
   GUI::Button m_gobackButton;

   // options page
   const Texture2D m_rightArrowTexture;
   std::array<Rectangle, 8> m_banners;
   const std::array<Texture, 8> m_bannerIcons;
   const std::array<std::string, 8> m_bannerNames;

   // settings page members
   GUI::Toggle m_darkModeToggle;

private:
   void resize(float boundsY);
   void setBannerPositions();

   void renderBlankBanners() const;
   void renderBannerContent() const;

   int getHoveredBannerIndex() const;
};