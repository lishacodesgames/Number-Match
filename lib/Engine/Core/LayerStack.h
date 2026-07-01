#pragma once
#include <vector>

#include "Layer.h"

namespace Core
{
   /// Owns all layers currently being processed in scene
   class LayerStack {
   public:
      LayerStack() = default;
      ~LayerStack() = default;
      
      /** @brief Deletes existing layers (in stack),
       * (must be done before Window closes or segmentation fault is thrown),
       * called by App only.
       */
      void Delete(); 

      void PushLayer(Layer* layer);
      void PopLayer(Layer* layer);
      
      void PushOverlay(Layer* overlay);
      void PopOverlay(Layer* overlay);

   public:
      // to allow range based forloops over the layerstack object itself
      using it = std::vector<Layer*>::iterator; 
      using r_it = std::vector<Layer*>::reverse_iterator; 

      // for const correctness
      using c_it = std::vector<Layer*>::const_iterator; 
      using cr_it = std::vector<Layer*>::const_reverse_iterator; 

      it begin() { return m_layers.begin(); }
      r_it rbegin() { return m_layers.rbegin(); }
      c_it cbegin() const { return m_layers.cbegin(); }
      cr_it crbegin() const { return m_layers.crbegin(); }

      it end() { return m_layers.end(); }
      r_it rend() { return m_layers.rend(); }
      c_it cend() const { return m_layers.cend(); }
      cr_it crend() const { return m_layers.crend(); }

      bool empty() const { return m_layers.empty(); }

   private:
      std::vector<Layer*> m_layers{};

      /// makes sure standard layers get placed before overlay layers
      size_t m_layerInsertIndex = 0;

   private:
      // for debug and error logging
      std::string formatLayerAddresses() const;
   };
}