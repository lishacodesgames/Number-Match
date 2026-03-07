#include <Precompiled.h>
#include "Game.h"

#include <raylib.h>
#include "Layers/HomeLayer.h"

Game* Game::s_instance = nullptr; // assign memory before assigning "this" ptr to it
Game::Game() {
   s_instance = this;

   InitWindow(800, 600, "Number Match");
   SetTargetFPS(60);

   PushLayer(new HomeLayer());
}

Game::~Game() { 
   m_layerStack.Delete(); /// Must be done before CloseWindow()
   CloseWindow(); 
}

Game& Game::Get() { return *s_instance; }

void Game::PushLayer(Layer* layer) { m_layerStack.PushLayer(layer); }

void Game::QueueLayerPush(Layer* layer) { m_pendingPushes.push_back(layer); }
void Game::QueueLayerPop(Layer* layer) { m_pendingPops.push_back(layer); }

void Game::OnEvent(Event& e) {
   // TOPMOST (last) layer must get the event FIRST
   for(auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it) {
      (*it)->OnEvent(e);
      if(e.Handled) 
         break; // stop propagating if event was handled
   }
}

void Game::Run() {
   while(!WindowShouldClose()) {
      // ---------------------------
      // 1. apply pending layer changes at the end of the current frame
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
