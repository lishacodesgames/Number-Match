#pragma once
#include <vector>
#include <string>
#include "Core/Layer.h"

namespace Core {
   /// Owns all layers currently being processed in scene
   class LayerStack {
   public:
      LayerStack() = default;
      ~LayerStack() = default;
      
      /** @brief Deletes existing layers (in sstack)
       * (must be done before Window closes or segmentation fault is thrown)
       * called by App only.
       */
      void Delete(); 

      void PushLayer(Layer* layer);
      void PopLayer(Layer* layer);
      
      void PushOverlay(Layer* overlay);
      void PopOverlay(Layer* overlay);

      // to allow range based forloops over the layerstack object itself
      std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
      std::vector<Layer*>::iterator end() { return m_layers.end(); }
      std::vector<Layer*>::reverse_iterator rbegin() { return m_layers.rbegin(); }
      std::vector<Layer*>::reverse_iterator rend() { return m_layers.rend(); }
      bool empty() const { return m_layers.empty(); }

   private:
      std::vector<Layer*> m_layers{};

      /// makes sure standard layers get placed before overlay layers
      size_t m_layerInsertIndex = 0;

   private:
      std::string formatLayerAddresses() const;
   };
}