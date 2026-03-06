#pragma once
#include <raylib.h>
#include <Button.h>
#include <Layer.h>

struct MenuPanel {
   MenuPanel();
   static constexpr float HEIGHT = 50.0f;

   Button homeButton;
   Button dailyButton;
   Button meButton;

   Button* findHoveredButton();
   Button* findActiveButton();
};

class MenuLayer : public Layer {
public:
   MenuLayer();
   ~MenuLayer() override;

   void OnAttach() override;
   void OnDetach() override;
   void OnUpdate() override;
   void OnEvent(Event &e) override;
   void OnRender() override;

private:
   Texture2D m_backgroundTexture;   
   Button m_startButton;
   MenuPanel m_panel;
};