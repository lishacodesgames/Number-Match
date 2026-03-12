#pragma once
#include "Button.h"
#include "Event.h"
#include "Layer.h"

#include <raylib.h>
#include <array>

class OptionsLayer : public Layer {
public:
   OptionsLayer();
   ~OptionsLayer() override = default;

   void OnAttach() override;
   void OnEvent(Event& e) override;
   void OnUpdate() override;
   void OnRender() override;
private:
   Texture m_rightArrowTexture = {0};

   Rectangle m_bounds;
   Button m_doneButton;

   enum BannerTypes {SETTINGS, HOW_TO, HELP, ABOUT, RIGHTS, PREFS, MATH, REMOVE_ADS};
   std::array<Rectangle, 8> m_banners;

   // render helpers
   void renderTopPanel();
   void renderBlankBanners();
   void renderBannerContent();
};