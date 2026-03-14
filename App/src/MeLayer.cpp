#include <pch/Precompiled.h>
#include "MeLayer.h"

#include <raylib.h>
#include "Core/Logging.h"
#include "Core/Layer.h"
#include "App.h"

MeLayer::MeLayer() : Core::Layer("Me Layer") {}
void MeLayer::OnAttach() {
   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
   Core::Layer::OnAttach();
}

void MeLayer::OnEvent(Core::Event& e) {
   if (e.GetEventType() == Core::EventType::KeyPressed)
      TraceLog(LISHA_SAYS, "The '%c' key was pressed", static_cast<Core::KeyPressedEvent&>(e).key);
   else if (e.GetEventType() == Core::EventType::MouseClicked)
      TraceLog(LISHA_SAYS, "Mouse was clicked");
}

void MeLayer::OnUpdate() {
   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void MeLayer::OnRender() {
   Vector2 textPos = {static_cast<float>(GetScreenWidth())/2-150, static_cast<float>(GetScreenHeight())/2-100};
   DrawTextEx(App::font_semibold, "Coming soon...", textPos, 35, 2.5f, DARKGRAY);
}