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

void MeLayer::OnUpdate() {
   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void MeLayer::OnRender() {
   Vector2 textPos = { GetScreenWidth() / 2.0f - 150, GetScreenHeight() / 2.0f - 100 };
   DrawTextEx(App::font_semibold, "Coming soon...", textPos, 35, 2.5f, TEXT_FOR_OFF_BRIGHT);
}