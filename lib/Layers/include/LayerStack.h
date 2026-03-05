#pragma once
#include <Layer.h>
#include <cstdint>
#include <vector>

class LayerStack {
public:
   LayerStack() = default;
   ~LayerStack();

   void PushLayer(Layer* layer);
   void PushOverlay(Layer* overlay);
   void PopLayer(Layer* layer);

   // to allow range based forloops over the layerstack object itself
   std::vector<Layer*>::iterator begin();
   std::vector<Layer*>::iterator end();
   std::vector<Layer*>::reverse_iterator rbegin();
   std::vector<Layer*>::reverse_iterator rend();

private:
   std::vector<Layer*> m_Layers{};

   /// makes sure standard layers get placed before overlay layers
   uint32_t m_layerInsertIndex = 0;
};