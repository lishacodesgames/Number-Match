#include <Precompiled.h>
#include "Layers/DailyLayer.h"

#include <raylib.h>
#include "Event.h"
#include "Layer.h"

DailyLayer::DailyLayer() : Layer("Daily Layer", false) {}

void DailyLayer::OnEvent(Event& e) {
   if(e.GetEventType() == EventType::KeyPressed)
      TraceLog(LOG_INFO, "LISHA SAYS: The '%c' key was pressed", static_cast<KeyPressedEvent&>(e).key);
   else if(e.GetEventType() == EventType::MouseClicked)
      TraceLog(LOG_INFO, "LISHA SAYS: Mouse was clicked");
}

void DailyLayer::OnUpdate() {}

void DailyLayer::OnRender() {
   DrawText("Coming soon...", GetScreenWidth()/2-150, GetScreenHeight()/2-100, 35, DARKGRAY);
}