#pragma once
#include "Button.h"
#include "Event.h"
#include "Layer.h"

#include <raylib.h>
#include <string>
#include <array>

class OptionsLayer : public Layer {
public:
   OptionsLayer();
   ~OptionsLayer() override;

   void OnAttach() override;
   void OnEvent(Event& e) override;
   void OnUpdate() override;
   void OnRender() override;
private:
   Texture m_rightArrowTexture = {0};

   Rectangle m_bounds;
   Button m_doneButton;

   enum BannerTypes {SETTINGS, HOW_TO, HELP, ABOUT, PRIVACY, PREFS, MATH, NO_ADS}; /// simply for more readability when looping over parts of the banners' arrays
   std::array<Rectangle, 8> m_banners;
   std::array<Texture, 8> m_bannerIcons;
   std::array<std::string, 8> m_bannerNames;

   // render helpers
   void renderTopPanel();
   void renderBlankBanners();
   void renderBannerContent();
};