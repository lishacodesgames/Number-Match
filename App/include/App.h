#pragma once
#include <raylib.h>
#include <vector>
#include <string>
#include "Core/LayerStack.h"
#include "Core/Event.h"
#include "Core/Layer.h"

constexpr Color LIGHTERGRAY = {230, 230, 230, 255}; /// A more aesthetically pleasing lightgray
constexpr Color MEDIUMGRAY = {180, 180, 180, 255}; /// Between raylib's LIGHTGRAY and GRAY
constexpr Color DARKERGRAY = {50, 50, 50, 255}; /// A more aesthetically pleasing darkgray

/// Static class that manages the game loop and all game states (menu, gameplay, pause, etc.) 
class App {
   static App* s_instance; // so App's statics and members can speak to each other
public:
   App(const std::string& name);
   ~App();
   
   void Run();
   
   static void QueueLayerSwap(Core::Layer* pop, Core::Layer* push);
   static void QueueLayerPush(Core::Layer* layer);
   static void QueueLayerPop(Core::Layer* layer);
   
   static Core::Layer* GetLayerByName(const std::string& name);

   static Font font_semibold;
   static Font font_black;
private:
   void OnEvent(Core::Event& e);

   Core::LayerStack m_layerStack;

   // memory management for pending layer changes, to be applied at the end of the current frame
   std::vector<Core::Layer*> m_pendingPushes{};
   std::vector<Core::Layer*> m_pendingPops{};
};