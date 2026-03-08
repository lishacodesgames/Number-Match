#include <Precompiled.h>
#include "Layers/MeLayer.h"

#include <raylib.h>
#include "Layer.h"

MeLayer::MeLayer() : Layer("Me Layer", false) {}

void MeLayer::OnEvent(Event& e) {}

void MeLayer::OnUpdate() {}

void MeLayer::OnRender() {
   DrawText("Coming soon...", GetScreenWidth()/2-150, GetScreenHeight()/2-100, 35, DARKGRAY);
}
