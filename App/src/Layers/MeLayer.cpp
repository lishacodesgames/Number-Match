#include <Precompiled.h>
#include "Layers/MeLayer.h"

#include <raylib.h>
#include "Layer.h"
#include "App.h"

MeLayer::MeLayer() : Layer("Me Layer", false) {}

void MeLayer::OnEvent(Event& e) {
   if(e.GetEventType() == EventType::KeyPressed)
      TraceLog(LOG_INFO, "LISHA SAYS: The '%c' key was pressed", static_cast<KeyPressedEvent&>(e).key);
   else if(e.GetEventType() == EventType::MouseClicked)
      TraceLog(LOG_INFO, "LISHA SAYS: Mouse was clicked");
}

void MeLayer::OnUpdate() {}

void MeLayer::OnRender() {
   Vector2 textPos = {static_cast<float>(GetScreenWidth())/2-150, static_cast<float>(GetScreenHeight())/2-100};
   DrawTextEx(App::font_semibold, "Coming soon...", textPos, 35, 2.5f, DARKGRAY);
}
