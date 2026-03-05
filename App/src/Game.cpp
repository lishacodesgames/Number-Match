#include <Precompiled.h>
#include "Game.h"

#include <raylib.h>
#include "MenuLayer.h"

Game* Game::s_instance = nullptr; // assign memory before assigning "this" ptr to it
Game::Game() {
   s_instance = this;

   InitWindow(800, 600, "Architectured Raylib Template");
   SetTargetFPS(60);

   PushLayer(new MenuLayer());
}

Game::~Game() { CloseWindow(); }

Game& Game::Get() { return *s_instance; }

void Game::PushLayer(Layer* layer) { m_layerStack.PushLayer(layer); }

void Game::QueueLayerSwap(Layer* pop_layer, Layer* push_layer) {
   m_pendingPop = pop_layer;
   m_pendingPush = push_layer;
}

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
      // 1. apply pending layer changes at the end of the current frame, to avoid mid-frame changes that could cause bugs
      if(m_pendingPop) {
         m_layerStack.PopLayer(m_pendingPop);
         delete m_pendingPop; // free memory of popped layer
         m_pendingPop = nullptr;
      }
      if(m_pendingPush) {
         m_layerStack.PushLayer(m_pendingPush);
         m_pendingPush = nullptr;
      }

      // 2. generate events
      
      // key event
      int key = GetKeyPressed();
      while(key != 0) {
         KeyPressedEvent e(key);
         OnEvent(e);
         key = GetKeyPressed();
      }

      // mouse event
      if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
         MouseClickedEvent e(MOUSE_LEFT_BUTTON);
         OnEvent(e);
      }

      // 3. update logic: bottom layer -> top layer, so that top layers can override logic of lower layers 
      // (eg. pause menu can override gameplay input)
      for(Layer* layer : m_layerStack)
         layer->OnUpdate();
      
      // 4. render: bottom layer -> top layer, so that top layers render on top of lower layers
      BeginDrawing();
      ClearBackground(Color{213, 255, 255, 255});

      for(Layer* layer : m_layerStack)
         layer->OnRender();

      EndDrawing();
   }
}
