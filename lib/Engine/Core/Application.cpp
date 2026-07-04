#include <pch/Precompiled.h>
#include "Application.h"

namespace Core
{
   void Application::Run() {
      Core::ConsoleLog(LISHA_SAYS, std::format("Working Directory: {}", GetWorkingDirectory()));

      while(!WindowShouldClose()) {
         // ---------------------------
         // 1. apply pending layer changes at the start of the current frame
         // to avoid mid-frame changes that could cause bugs
         // ---------------------------
         for(Core::Layer* layer : m_pendingPops)
            layer->isOverlay ? m_layerStack.PopOverlay(layer) : m_layerStack.PopLayer(layer);
         m_pendingPops.clear();

         for(Core::Layer* layer : m_pendingPushes)
            layer->isOverlay ? m_layerStack.PushOverlay(layer): m_layerStack.PushLayer(layer);
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

         // resize event
         if(IsWindowResized()) {
            Core::WindowResizeEvent e(GetScreenWidth(), GetScreenHeight());
            OnEvent(e);
         }

         // ---------------------------
         // 3. update logic: bottom layer -> top layer
         // so that top layers can override logic of lower layers 
         // (eg. pause menu can override gameplay input)
         // ---------------------------
         
         for(Core::Layer* layer : m_layerStack) {
            if(layer->isSuspended && !layer->updateSuspended)
               continue;

            layer->OnUpdate();
         }
         
         // ---------------------------
         // 4. render: bottom layer -> top layer
         // so that top layers render on top of lower layers
         // ---------------------------
         
         BeginDrawing();
         ClearBackground(*bgColor);

         for(Core::Layer* layer : m_layerStack) {
            if(layer->isSuspended && !layer->renderSuspended)
               continue;

            layer->OnRender();
         }

         EndDrawing();
      }
   }

   void Application::QueueLayerSwap(Core::Layer* pop, Core::Layer* push) {
      QueueLayerPop(pop);
      QueueLayerPush(push);
   }

   void Application::QueueLayerPush(Core::Layer* layer) {
      // if layer already exists, pop it first
      for(Core::Layer* existingLayer : s_instance->m_layerStack) {
         if(existingLayer->GetName() == layer->GetName()) {
            QueueLayerPop(existingLayer);
            break;
         }
      }

      s_instance->m_pendingPushes.push_back(layer);
   }

   void Application::QueueLayerPop(Core::Layer* layer) {
      s_instance->m_pendingPops.push_back(layer);
   }

   Core::Layer* Application::GetLayerByName(std::string_view name) {
      for(Core::Layer* layer : s_instance->m_layerStack)
         if(layer->GetName() == name)
            return layer;

      LOG_LAYER("Layer \"{}\" doesn't exist.", name);
      return nullptr;
   }

   void Application::OnEvent(Core::Event& e) {
      if(e.GetEventType() == Core::EventType::WindowResize) {
         LOG_RESIZE("Window -> {} x {}", GetScreenWidth(), GetScreenHeight());

         // separate bcz WindowResizeEvent must not be stopped be e.Handled
         for(Core::Layer* layer : m_layerStack) {
            layer->OnEvent(e);
            if(layer->isSuspended && layer->renderSuspended) // otherwise the usual OnRender() will handle it
               layer->OnRender();
         }
      } else {
         // TOPMOST (last) layer must get the event FIRST
         // Hence we iterate backwards
         for(Core::Layer* layer : std::views::reverse(m_layerStack)) {
            if(layer->isSuspended && !layer->eventSuspended)
               continue;
      
            layer->OnEvent(e);
            if(e.Handled) 
               break; // stop propagating if event was handled
         }
      }
   }
}