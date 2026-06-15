#include <pch/Precompiled.h>
#include "MeLayer.h"

#include "Colors.h"
#include "App.h"

void MeLayer::OnEvent(Core::Event& e) {
   if(e.GetEventType() == Core::EventType::KeyPressed)
      TraceLog(LISHA_SAYS, "The '%c' key was pressed", static_cast<Core::KeyPressedEvent&>(e).key);
   else if(e.GetEventType() == Core::EventType::MouseClicked)
      TraceLog(LISHA_SAYS, "Mouse was clicked");
}

void MeLayer::OnUpdate() {}

void MeLayer::OnRender() {
   DrawTextEx(
      App::font_semibold, "Coming soon...",
      { GetScreenWidth() / 2.0f - 150, GetScreenHeight() / 2.0f - 100 },
      35, 2.5f, Palette::text_for_off_bright);
}