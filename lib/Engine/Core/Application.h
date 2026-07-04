#pragma once
#include "LayerStack.h"

namespace Core
{
   class Application {
   public:
      Application() { s_instance = this; }
      ~Application() { DestroyApplication(*this); }

      friend Application* CreateApplication(const std::string& name); /// must be defined by user
      friend void DestroyApplication(Application& app); /// must be defined by user, must handle ALL resources owned by this application (layers, windows, etc.)

      void Run();

      static void QueueLayerSwap(Core::Layer* pop, Core::Layer* push);
      static void QueueLayerPush(Core::Layer* layer); // Pops existing layer if pushed layer is a duplicate
      static void QueueLayerPop(Core::Layer* layer);

      static Core::Layer* GetLayerByName(std::string_view name);
   private:
      inline static Application* s_instance = nullptr; // so Application's statics and members can speak to each other
      Core::LayerStack m_layerStack;
      Color* bgColor = nullptr;

      // memory management for pending layer changes, to be applied at the end of the current frame
      std::vector<Core::Layer*> m_pendingPushes{};
      std::vector<Core::Layer*> m_pendingPops{};
   
   private:
      void OnEvent(Core::Event& e);
   };

   extern Application* CreateApplication(const std::string& name);
   extern void DestroyApplication(Application& app);
}