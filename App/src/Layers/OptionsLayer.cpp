#include <Precompiled.h>
#include "Layers/OptionsLayer.h"

#include "Event.h"
#include "App.h"

static constexpr int PANEL_HEIGHT = 35;
static constexpr float BANNER_HEIGHT = 30;

OptionsLayer::OptionsLayer() : Layer("Options Layer", true),
      m_bounds({
         200, 100, 
         static_cast<float>(GetScreenWidth()) / 2, 
         static_cast<float>(GetScreenHeight()) / 2 + 100
      }),
      m_doneButton(
         {m_bounds.x + m_bounds.width - 55, m_bounds.y + 7}, // origin 
         {0, 0}, "Done", BLANK, BLUE, 20, {0, 0}, App::font_semibold
      )
{
   float originX = m_bounds.x + m_bounds.width * (15.0f/100.0f)/2.0f; // half of 15%
   float originY = m_bounds.y + PANEL_HEIGHT * 1.5f;
   Vector2 size = {m_bounds.width * 0.85f, BANNER_HEIGHT};

   constexpr float spacing = BANNER_HEIGHT + 20;

   m_banners[SETTINGS] = {originX, originY, size.x, size.y};

   m_banners[HOW_TO] = {originX, originY + spacing, size.x, size.y};
   m_banners[HELP] = {originX, originY + spacing + BANNER_HEIGHT, size.x, size.y};
   m_banners[ABOUT] = {originX, originY + spacing + BANNER_HEIGHT*2, size.x, size.y};
   m_banners[RIGHTS] = {originX, originY + spacing + BANNER_HEIGHT*3, size.x, size.y};
   m_banners[PREFS] = {originX, originY + spacing + BANNER_HEIGHT*4, size.x, size.y};

   m_banners[PUZZLE] = {originX, originY + spacing*2 + BANNER_HEIGHT*4, size.x, size.y};
   m_banners[REMOVE_ADS] = {originX, originY + spacing*3 + BANNER_HEIGHT*4, size.x, size.y};
}

void OptionsLayer::OnAttach() {
   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
   Layer::OnAttach();
}

void OptionsLayer::OnEvent(Event& e) {
   e.Handled = true; // don't want any events to pass through to gamelayer
   if(e.GetEventType() == EventType::MouseClicked && m_doneButton.isHovered) {
      App::GetLayerByName("Game Layer")->OnResume(); // we're sure that game exists bc OptionsLayer only exists in its context
      App::QueueLayerPop(this);
   }
}

void OptionsLayer::OnUpdate() {
   m_doneButton.Update(); 

   if(m_doneButton.isHovered)
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void OptionsLayer::OnRender() {
   DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {80, 80, 80, 180}); // to make the bg darker

   DrawRectangleRounded(m_bounds, 0.1f, 6, LIGHTGRAY); // main popup
   
   // top panel
   Rectangle panel = m_bounds;
   panel.height = PANEL_HEIGHT;
   
   Rectangle panelSharpBottom = panel;
   panelSharpBottom.y += PANEL_HEIGHT/2;
   panelSharpBottom.height = PANEL_HEIGHT/2;

   DrawRectangleRounded(panel, 0.8f, 6, WHITE);
   DrawRectangleRec(panelSharpBottom, WHITE);
   DrawTextEx(
      App::font_semibold, "Options", 
      {m_bounds.x + m_bounds.width/2 - 30, m_bounds.y + 7}, 
      20, 1, BLACK
   );
   m_doneButton.Draw();

   // banners
   for(Rectangle banner : m_banners) {
      DrawRectangleRounded(banner, 0.85f, 4, WHITE);
      DrawRectangleRoundedLines(banner, 0.85f, 4, GRAY);
   }
}