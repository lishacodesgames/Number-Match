#include <Precompiled.h>
#include "Layers/MeLayer.h"

#include <raylib.h>
#include "Layer.h"

MeLayer::MeLayer() : Layer("Me Layer", false) {}

void MeLayer::OnEvent(Event& e) {
   if(e.GetEventType() == EventType::KeyPressed)
      TraceLog(LOG_INFO, "LISHA SAYS: The '%c' key was pressed", static_cast<KeyPressedEvent&>(e).key);
   else if(e.GetEventType() == EventType::MouseClicked)
      TraceLog(LOG_INFO, "LISHA SAYS: Mouse was clicked");
}

void MeLayer::OnUpdate() {}

void MeLayer::OnRender() {
   DrawText("Coming soon...", GetScreenWidth()/2-150, GetScreenHeight()/2-100, 35, DARKGRAY);
}
