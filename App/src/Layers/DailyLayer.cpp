#include <Precompiled.h>
#include "Layers/DailyLayer.h"

#include <raylib.h>
#include "Layer.h"

DailyLayer::DailyLayer() : Layer("Daily Layer", false) {}

void DailyLayer::OnEvent(Event& e) {}

void DailyLayer::OnUpdate() {}

void DailyLayer::OnRender() {
   DrawText("Coming soon...", GetScreenWidth()/2-150, GetScreenHeight()/2-100, 35, DARKGRAY);
}