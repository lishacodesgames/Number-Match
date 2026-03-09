#pragma once
#include <raylib.h>
#include <vector>
#include <string>
#include "LayerStack.h"
#include "Event.h"
#include "Layer.h"

/// Static class that manages the game loop and all game states (menu, gameplay, pause, etc.) 
class App {
   static App* s_instance; // so App's statics and members can speak to each other
public:
   App(const std::string& name);
   ~App();
   
   void Run();
   
   static void QueueLayerSwap(Layer* pop, Layer* push);
   static void QueueLayerPush(Layer* layer);
   static void QueueLayerPop(Layer* layer);
   
   static Layer* GetLayerByName(const std::string& name);

   static Font font_semibold;
   static Font font_black;
private:
   void OnEvent(Event& e);

   LayerStack m_layerStack;

   // memory management for pending layer changes, to be applied at the end of the current frame
   std::vector<Layer*> m_pendingPushes{};
   std::vector<Layer*> m_pendingPops{};
};