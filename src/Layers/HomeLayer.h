#pragma once
#include <raylib.h>
#include "Core/Layer.h"
#include "GUI/Button.h"

class HomeLayer : public Core::Layer {
public:
   HomeLayer();
   ~HomeLayer() override {
      UnloadTexture(m_trophyTexture);
      UnloadTexture(m_backgroundTexture);
   }

   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;

private:
   const Texture m_backgroundTexture, m_trophyTexture;
   float m_bgScale, m_trophyScale;

   GUI::Button m_newButton;
   GUI::Button m_continueButton;

private:
   void resize();

   /// @return new or continue button members
   GUI::Button* findHoveredButton() {
      if(m_newButton.isHovered)
         return &m_newButton;
      else if(m_continueButton.isHovered)
         return &m_continueButton;
      else
         return nullptr;
   }
};

inline Vector2 operator+(Vector2 vec, float fl) { return { vec.x + fl, vec.y + fl }; }