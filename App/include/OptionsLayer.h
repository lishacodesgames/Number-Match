#pragma once
#include "Core/Event.h"
#include "Core/Layer.h"
#include "GUI/Button.h"

#include <raylib.h>
#include <string>
#include <array>

class OptionsLayer : public Core::Layer {
public:
   OptionsLayer();
   ~OptionsLayer() override;

   void OnAttach() override;
   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;
private:
   Texture m_rightArrowTexture = {0};

   Rectangle m_bounds;
   GUI::Button m_doneButton;

   std::array<Rectangle, 8> m_banners;
   std::array<Texture, 8> m_bannerIcons;
   std::array<std::string, 8> m_bannerNames;

   /// simply for more readability when looping over parts of the banners' arrays
   enum BannerTypes {SETTINGS, HOW_TO, HELP, ABOUT, PRIVACY, PREFS, MATH, NO_ADS}; 

   // helpers
   void setBannerPositions(float boundsY);
   void renderTopPanel();
   void renderBlankBanners();
   void renderBannerContent();
};