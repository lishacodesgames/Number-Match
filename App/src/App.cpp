#include <Precompiled.h>
#include "App.h"

#include <algorithm>
#include <typeinfo>
#include <raylib.h>
#include <cstdio>
#include "Layers/PanelLayer.h"
#include "Layers/HomeLayer.h"

Font App::font_semibold = GetFontDefault(); // font must be loaded after InitWindow()
Font App::font_black = GetFontDefault();

App* App::s_instance = nullptr;

App::App(const std::string& name) {
   s_instance = this;

   TraceLog(LOG_INFO, "LISHA SAYS: Loading App...");

   InitWindow(800, 600, name.c_str());
   SetTargetFPS(60);

   font_semibold = LoadFontEx("assets/RedHatDisplay-SemiBold.ttf", 40, NULL, 0);
   font_black = LoadFontEx("assets/RedHatDisplay-Black.ttf", 70, NULL, 0);

   HomeLayer* home = new HomeLayer();
   m_layerStack.PushLayer(home);
   PanelLayer* panel = new PanelLayer();
   m_layerStack.PushOverlay(panel);

   panel->currentLayer = home;
   TraceLog(LOG_INFO, "LISHA SAYS: App Loaded!");
}

App::~App() { 
   m_layerStack.Delete(); /// Must be done before CloseWindow()
   CloseWindow();
   s_instance = nullptr;
   printf("LISHA SAYS: GOODBYE!\n");
}

void App::QueueLayerSwap(Layer* pop, Layer* push) {
   QueueLayerPop(pop);
   QueueLayerPush(push);
}

void App::QueueLayerPush(Layer* layer) {
   for(Layer* existing : s_instance->m_layerStack)
      if(typeid(*existing) == typeid(*layer)) // duplicate layers
         QueueLayerPop(existing);

   s_instance->m_pendingPushes.push_back(layer);
}
void App::QueueLayerPop(Layer* layer) { s_instance->m_pendingPops.push_back(layer); }

Layer* App::GetLayerByName(const std::string& name) {
   for(Layer* layer : s_instance->m_layerStack)
      if(layer->GetName() == name)
         return layer;
   
   return nullptr;
}

void App::OnEvent(Event& e) {
   // TOPMOST (last) layer must get the event FIRST
   for(auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it) {
      (*it)->OnEvent(e);
      if(e.Handled) 
         break; // stop propagating if event was handled
   }
}

void App::Run() {
   TraceLog(LOG_INFO, "LISHA SAYS: Working Directory: %s", GetWorkingDirectory());

   while(!WindowShouldClose()) {
      // ---------------------------
      // 1. apply pending layer changes at the start of the current frame
      // to avoid mid-frame changes that could cause bugs
      // ---------------------------
      for(Layer* layer : m_pendingPops) {
         layer->isOverlay? m_layerStack.PopOverlay(layer) : m_layerStack.PopLayer(layer);
         delete layer; // free memory of popped layer
         layer = nullptr;
      }
      m_pendingPops.clear();

      for(Layer* layer : m_pendingPushes) {
         layer->isOverlay? m_layerStack.PushOverlay(layer): m_layerStack.PushLayer(layer);
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
