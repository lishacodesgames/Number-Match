#include <Precompiled.h>
#include "Game.h"

#include <algorithm>
#include <typeinfo>
#include <raylib.h>
#include "Layers/HomeLayer.h"

App* App::s_instance = nullptr; // assign memory before assigning "this" ptr to it
App::App() {
   s_instance = this;

   InitWindow(800, 600, "Number Match");
   SetTargetFPS(60);

   m_layerStack.PushLayer(new HomeLayer());
}

App::~App() { 
   m_layerStack.Delete(); /// Must be done before CloseWindow()
   CloseWindow(); 
}
App& App::Get() { return *s_instance; }

void App::QueueLayerPush(Layer* layer) {
   for(auto* existing : m_layerStack) {
      if(typeid(*existing) == typeid(*layer)) // duplicate layers
         QueueLayerPop(existing);
   }

   m_pendingPushes.push_back(layer);
}
void App::QueueLayerPop(Layer* layer) { m_pendingPops.push_back(layer); }

void App::OnEvent(Event& e) {
   // TOPMOST (last) layer must get the event FIRST
   for(auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it) {
      (*it)->OnEvent(e);
      if(e.Handled) 
         break; // stop propagating if event was handled
   }
}

void App::Run() {
   while(!WindowShouldClose()) {
      // ---------------------------
      // 1. apply pending layer changes at the start of the current frame
      // to avoid mid-frame changes that could cause bugs
      // ---------------------------
      for(Layer* layer : m_pendingPops) {
         m_layerStack.PopLayer(layer);
         delete layer; // free memory of popped layer
         layer = nullptr;
      }
      m_pendingPops.clear();

      for(Layer* layer : m_pendingPushes) {
         m_layerStack.PushLayer(layer);
         layer = nullptr;
      }
      m_pendingPushes.clear();

      // ---------------------------
      // 2. generate events
      // ---------------------------
      
      // key event
      int key = GetKeyPressed();
      while(key != 0) {
         KeyPressedEvent e(key);
         OnEvent(e);
         key = GetKeyPressed();
      }

      // mouse event
      if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
         MouseClickedEvent e(true);
         OnEvent(e);
      }

      // ---------------------------
      // 3. update logic: bottom layer -> top layer
      // so that top layers can override logic of lower layers 
      // (eg. pause menu can override gameplay input)
      // ---------------------------
      
      for(Layer* layer : m_layerStack)
         layer->OnUpdate();
      
      // ---------------------------
      // 4. render: bottom layer -> top layer
      // so that top layers render on top of lower layers
      // ---------------------------
      
      BeginDrawing();
      ClearBackground(RAYWHITE);

      for(Layer* layer : m_layerStack)
         layer->OnRender();

      EndDrawing();
   }
}
