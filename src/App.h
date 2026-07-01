#pragma once
#include "Core/LayerStack.h"
#include "Core/Layer.h"

#include <raylib.h>

/// Static class that manages the game loop and all game states (menu, gameplay, pause, etc.)
class App {
public:
   App(const std::string& name);
   ~App();
   
   void Run();
   
   static void QueueLayerSwap(Core::Layer* pop, Core::Layer* push);
   static void QueueLayerPush(Core::Layer* layer); // Pops existing layer if pushed layer is a duplicate
   static void QueueLayerPop(Core::Layer* layer);
   
   static Core::Layer* GetLayerByName(const std::string& name);

public:
   static Font font_retro;
   static Font font_semibold;
   static Font font_black;

private:
   static App* s_instance; // so App's statics and members can speak to each other
   Core::LayerStack m_layerStack;

   // memory management for pending layer changes, to be applied at the end of the current frame
   std::vector<Core::Layer*> m_pendingPushes{};
   std::vector<Core::Layer*> m_pendingPops{};
   
private:
   void OnEvent(Core::Event& e);
};