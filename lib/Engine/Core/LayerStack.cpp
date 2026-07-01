#include <pch/Precompiled.h>
#include "LayerStack.h"

namespace Core
{
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
         delete layer;
      } else {
         TraceLog(LOG_FATAL, "\n%s", formatLayerAddresses().c_str());
         throw std::runtime_error(
            std::format("Tried to pop a layer that doesn't exist!\nLayer: {}, Address: {}",
               layer->GetName(), static_cast<const void*>(layer))
         );
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
         delete overlay;
      } else {
         TraceLog(LOG_FATAL, "\n%s", formatLayerAddresses().c_str());
         throw std::runtime_error(
            std::format("Tried to pop an overlay layer that doesn't exist!\nLayer: {}, Address: {}",
               overlay->GetName(), static_cast<const void*>(overlay))
         );
      }
   }

   std::string LayerStack::formatLayerAddresses() const {
      std::string layers = std::format("Layers in LayerStack: {}\n", m_layers.size());
      for(size_t i = 0; i < m_layers.size(); i++)
         layers += std::format("\tLayerStack[{}]: {}, {}\n", i, m_layers.at(i)->GetName(), static_cast<const void*>(m_layers.at(i)));

      return layers;
   }

   void LayerStack::Delete() {
      for(Layer* layer : m_layers)
         delete layer;
         
      m_layers.clear();
      m_layerInsertIndex = 0;
   }
} // namespace Core