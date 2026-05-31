#include <pch/Precompiled.h>
#include "App.h"

#include "Core/Logging.h"
#include "PanelLayer.h"
#include "HomeLayer.h"
#include "CoinLayer.h"

Font App::font_semibold = GetFontDefault(); // font must be loaded after InitWindow()
Font App::font_black = GetFontDefault();

App* App::s_instance = nullptr;

App::App(const std::string& name) {
   s_instance = this;

   TraceLog(LISHA_SAYS, "Loading App...");

   SetConfigFlags(FLAG_WINDOW_RESIZABLE);
   InitWindow(800, 650, name.c_str());
   SetWindowMinSize(500, 500);
   SetTargetFPS(60);

   font_semibold = LoadFontEx("assets/fonts/RedHatDisplay-SemiBold.ttf", 40, NULL, 0);
   font_black = LoadFontEx("assets/fonts/RedHatDisplay-Black.ttf", 70, NULL, 0);

   HomeLayer* home = new HomeLayer();
   m_layerStack.PushLayer(home);
   PanelLayer* panel = new PanelLayer();
   m_layerStack.PushOverlay(panel);
   panel->currentLayer = home;
   
   m_layerStack.PushOverlay(new CoinLayer());
   TraceLog(LISHA_SAYS, "App Loaded!");
}

App::~App() { 
   m_layerStack.Delete(); /// Must be done before CloseWindow()
   CloseWindow();
   s_instance = nullptr;
   TraceLog(LISHA_SAYS, "GOODBYE!\n");
}

void App::Run() {
   TraceLog(LISHA_SAYS, "Working Directory: %s", GetWorkingDirectory());

   while(!WindowShouldClose()) {
      // ---------------------------
      // 1. apply pending layer changes at the start of the current frame
      // to avoid mid-frame changes that could cause bugs
      // ---------------------------
      for(Core::Layer* layer : m_pendingPops) {
         layer->isOverlay? m_layerStack.PopOverlay(layer) : m_layerStack.PopLayer(layer);
         delete layer; // free memory of popped layer
      }
      m_pendingPops.clear();

      for(Core::Layer* layer : m_pendingPushes)
         layer->isOverlay? m_layerStack.PushOverlay(layer): m_layerStack.PushLayer(layer);
      m_pendingPushes.clear();

      // ---------------------------
      // 2. generate events
      // ---------------------------
      
      // key event
      int key = GetKeyPressed();
      while(key != 0) {
         Core::KeyPressedEvent e(key);
         OnEvent(e);
         key = GetKeyPressed();
      }

      // mouse event
      if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
         Core::MouseClickedEvent e(true);
         OnEvent(e);
      }

      // ---------------------------
      // 3. update logic: bottom layer -> top layer
      // so that top layers can override logic of lower layers 
      // (eg. pause menu can override gameplay input)
      // ---------------------------
      
      for(Core::Layer* layer : m_layerStack)
         layer->OnUpdate();
      
      // ---------------------------
      // 4. render: bottom layer -> top layer
      // so that top layers render on top of lower layers
      // ---------------------------
      
      BeginDrawing();
      ClearBackground(RAYWHITE);

      for(Core::Layer* layer : m_layerStack)
         layer->OnRender();

      EndDrawing();
   }
}

void App::QueueLayerSwap(Core::Layer* pop, Core::Layer* push) {
   QueueLayerPop(pop);
   QueueLayerPush(push);
}

void App::QueueLayerPush(Core::Layer* layer) {
   for(Core::Layer* existing : s_instance->m_layerStack)
      if(typeid(*existing) == typeid(*layer)) // duplicate layers
         QueueLayerPop(existing);

   s_instance->m_pendingPushes.push_back(layer);
}
void App::QueueLayerPop(Core::Layer* layer) { s_instance->m_pendingPops.push_back(layer); }

Core::Layer* App::GetLayerByName(const std::string& name) {
   for(Core::Layer* layer : s_instance->m_layerStack)
      if(layer->GetName() == name)
         return layer;
   
   return nullptr;
}

void App::OnEvent(Core::Event& e) {
   // TOPMOST (last) layer must get the event FIRST
   for(auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it) {
      (*it)->OnEvent(e);
      if(e.Handled) 
         break; // stop propagating if event was handled
   }
}
