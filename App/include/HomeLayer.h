#pragma once
#include <raylib.h>
#include "PanelLayer.h"
#include "Core/Layer.h"
#include "GUI/Button.h"

class HomeLayer : public Core::Layer {
public:
   HomeLayer();
   ~HomeLayer() override { UnloadTexture(m_backgroundTexture); }

   void OnEvent(Core::Event &e) override;
   void OnUpdate() override;
   void OnRender() override;
private:
   Image m_backgroundImage = {0};
   Texture m_backgroundTexture = {0};
   Texture m_trophyTexture = {0};

   GUI::Button m_newButton;
   GUI::Button m_continueButton;
private:
   GUI::Button* findHoveredButton() {
      if(m_newButton.isHovered)
         return &m_newButton;
      else if(m_continueButton.isHovered)
         return &m_continueButton;
      else
         return nullptr;
   }

   void resizeButtons();
   void resizeBackground();
};

inline Vector2 operator+(const Vector2& vec, const float& fl) { return {vec.x+fl, vec.y+fl}; }