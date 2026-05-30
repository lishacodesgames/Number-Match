#include <pch/Precompiled.h>
#include "DailyLayer.h"

#include "Core/Logging.h"
#include "App.h"

void DailyLayer::OnEvent(Core::Event& e) {
   if(e.GetEventType() == Core::EventType::KeyPressed)
      TraceLog(LISHA_SAYS, "The '%c' key was pressed", static_cast<Core::KeyPressedEvent&>(e).key);
   else if(e.GetEventType() == Core::EventType::MouseClicked)
      TraceLog(LISHA_SAYS, "Mouse was clicked");
}

void DailyLayer::OnUpdate() {
   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void DailyLayer::OnRender() {
   Vector2 textPos = {static_cast<float>(GetScreenWidth())/2-150, static_cast<float>(GetScreenHeight())/2-100};
   DrawTextEx(App::font_semibold, "Coming soon...", textPos, 35, 2.5f, DARKGRAY);
}