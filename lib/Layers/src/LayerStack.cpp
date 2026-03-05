#include <Precompiled.h>
#include <LayerStack.h>

#include <algorithm>
#include <Layer.h>
#include <vector>

LayerStack::~LayerStack() {
   for(int i = 0; i < m_Layers.size(); i++) {
      delete m_Layers[i];
   }
}

void LayerStack::PushLayer(Layer* layer) {
   m_Layers.emplace(begin() + m_layerInsertIndex, layer);
   m_layerInsertIndex++;
   
   layer->OnAttach();
}

void LayerStack::PushOverlay(Layer* overlay) {
   m_Layers.emplace_back(overlay);
   overlay->OnAttach();
}

void LayerStack::PopLayer(Layer* layer) {
   std::vector<Layer*>::iterator it = 
      std::find(begin(), end(), layer);

   if(it != end()) {
      layer->OnDetach();
      m_Layers.erase(it);
      m_layerInsertIndex--;
   }
}

std::vector<Layer*>::iterator LayerStack::begin() { return m_Layers.begin(); }
std::vector<Layer*>::iterator LayerStack::end() { return m_Layers.end(); }
std::vector<Layer*>::reverse_iterator LayerStack::rbegin() { return m_Layers.rbegin(); }
std::vector<Layer*>::reverse_iterator LayerStack::rend() { return m_Layers.rend(); }
