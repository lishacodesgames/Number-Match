#pragma once
#include <raylib.h>
#include "Core/Layer.h"
#include "GUI/Button.h"

class HomeLayer : public Core::Layer {
public:
   HomeLayer();
   ~HomeLayer() override {
      UnloadImage(m_trophyImage);
      UnloadImage(m_backgroundImage);
      UnloadTexture(m_trophyTexture);
      UnloadTexture(m_backgroundTexture);
   }

   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;

private:
   Image m_backgroundImage = { 0 };
   Texture m_backgroundTexture = { 0 };

   Image m_trophyImage = { 0 };
   Texture m_trophyTexture = { 0 };

   GUI::Button m_newButton;
   GUI::Button m_continueButton;

private:
   void resize();

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