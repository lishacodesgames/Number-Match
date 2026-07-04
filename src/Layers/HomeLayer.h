#pragma once
#include "Core/Layer.h"
#include "GUI/Button.h"

class HomeLayer : public Core::Layer {
public:
   HomeLayer();
   ~HomeLayer() override {
      UnloadTexture(m_trophyTexture);
      UnloadTexture(m_backgroundTexture);
   }

   void OnDetach() override {
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
      Core::Layer::OnDetach();
   }

   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;

private:
   const Texture2D m_backgroundTexture, m_trophyTexture;
   float m_bgScale = 1.0f, m_trophyScale = 1.0f;

   GUI::Button m_newButton;
   GUI::Button m_continueButton;

private:
   void resize();

   /// @return new or continue button members OR nullptr
   GUI::Button* findHoveredButton();
};

/// @todo should be in raymath.h but move to Engine/
inline Vector2 operator+(Vector2 vec, float fl) { return { vec.x + fl, vec.y + fl }; }