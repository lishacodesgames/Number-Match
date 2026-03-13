#include <pch/Precompiled.h>
#include "Layers/DailyLayer.h"

#include <raylib.h>
#include "Core/Logging.h"
#include "Core/Event.h"
#include "Core/Layer.h"
#include "App.h"

DailyLayer::DailyLayer() : Layer("Daily Layer") {}
void DailyLayer::OnAttach() {
   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
   Layer::OnAttach();
}

void DailyLayer::OnEvent(Event& e) {
   if(e.GetEventType() == EventType::KeyPressed)
      TraceLog(LISHA_SAYS, "The '%c' key was pressed", static_cast<KeyPressedEvent&>(e).key);
   else if(e.GetEventType() == EventType::MouseClicked)
      TraceLog(LISHA_SAYS, "Mouse was clicked");
}

void DailyLayer::OnUpdate() {
   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void DailyLayer::OnRender() {
   Vector2 textPos = {static_cast<float>(GetScreenWidth())/2-150, static_cast<float>(GetScreenHeight())/2-100};
   DrawTextEx(App::font_semibold, "Coming soon...", textPos, 35, 2.5f, DARKGRAY);
}