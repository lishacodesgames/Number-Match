#include <pch/Precompiled.h>
#include "Core/LayerStack.h"

#include <algorithm>
#include <vector>
#include "Core/Layer.h"

namespace Core {
   void LayerStack::PushLayer(Layer* layer) {
      m_layers.emplace(begin() + m_layerInsertIndex, layer);
      m_layerInsertIndex++;
      
      layer->OnAttach();
   }

   void LayerStack::PopLayer(Layer* layer) {
      auto it = std::find(begin(), begin() + m_layerInsertIndex, layer);

      if(it != begin() + m_layerInsertIndex) {
         layer->OnDetach();
         m_layers.erase(it);
         m_layerInsertIndex--;
      }
   }

   void LayerStack::PushOverlay(Layer* overlay) {
      m_layers.emplace_back(overlay);
      overlay->OnAttach();
   }

   void LayerStack::PopOverlay(Layer* overlay) {
      auto it = std::find(begin() + m_layerInsertIndex, end(), overlay);

      if(it != end()) {
         overlay->OnDetach();
         m_layers.erase(it);
      }
   }

   std::vector<Layer*>::iterator LayerStack::begin() { return m_layers.begin(); }
   std::vector<Layer*>::iterator LayerStack::end() { return m_layers.end(); }
   std::vector<Layer*>::reverse_iterator LayerStack::rbegin() { return m_layers.rbegin(); }
   std::vector<Layer*>::reverse_iterator LayerStack::rend() { return m_layers.rend(); }
   bool LayerStack::empty() const { return m_layers.empty(); }

   void LayerStack::Delete() {
      for(Layer* layer : m_layers)
         delete layer;
         
      m_layers.clear();
      m_layerInsertIndex = 0;
   }
} // namespace Core