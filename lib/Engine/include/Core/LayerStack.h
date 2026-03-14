#pragma once
#include <vector>
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
      std::vector<Layer*>::iterator begin();
      std::vector<Layer*>::iterator end();
      std::vector<Layer*>::reverse_iterator rbegin();
      std::vector<Layer*>::reverse_iterator rend();
      bool empty() const;

   private:
      std::vector<Layer*> m_layers{};

      /// makes sure standard layers get placed before overlay layers
      size_t m_layerInsertIndex = 0;
   };
}