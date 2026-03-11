#include <Precompiled.h>
#include "Layers/OptionsLayer.h"

#include "Event.h"
#include "App.h"

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
{}

void OptionsLayer::OnEvent(Event& e) {
   if(e.GetEventType() == EventType::MouseClicked && m_doneButton.isHovered)
      App::QueueLayerPop(this);
}

void OptionsLayer::OnUpdate() {
   m_doneButton.Update(); 

   if(m_doneButton.isHovered)
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
}

void OptionsLayer::OnRender() {
   DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {80, 80, 80, 180}); // to make the bg darker

   DrawRectangleRounded(m_bounds, 0.1f, 6, LIGHTGRAY); // main popup
   
   // top panel
   Rectangle panel = m_bounds;
   panel.height = 35;
   
   Rectangle panelSharpBottom = panel;
   panelSharpBottom.y += panel.height/2;
   panelSharpBottom.height /= 2;

   DrawRectangleRounded(panel, 0.8f, 6, WHITE);
   DrawRectangleRec(panelSharpBottom, WHITE);
   DrawTextEx(
      App::font_semibold, "Options", 
      {m_bounds.x + m_bounds.width/2 - 30, m_bounds.y + 7}, 
      20, 1, BLACK
   );
   m_doneButton.Draw();
}