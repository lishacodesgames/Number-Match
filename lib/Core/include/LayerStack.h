#pragma once
#include <cstdint>
#include <vector>
#include "Layer.h"

class LayerStack {
public:
   LayerStack() = default;
   ~LayerStack() = default;
   void Delete(); /// Deletes all layers (must be done before Window closes or segmentation fault is thrown)

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
   uint32_t m_layerInsertIndex = 0;
};