#pragma once
#include <LayerStack.h>
#include <raylib.h>
#include <Layer.h>
#include <Event.h>
#include <vector>

/// Singleton class that manages the game loop and all game states (menu, gameplay, pause, etc.) 
class Game {
   static Game* s_instance; // static singleton instance
public:
   Game();
   ~Game();

   static Game& Get();

   void PushLayer(Layer* layer);
   void QueueLayerPush(Layer* layer);
   void QueueLayerPop(Layer* layer);

   void OnEvent(Event& e);
   void Run();
private: // members
   LayerStack m_layerStack;

   // memory management for pending layer changes, to be applied at the end of the current frame
   std::vector<Layer*> m_pendingPushes{};
   std::vector<Layer*> m_pendingPops{};
};