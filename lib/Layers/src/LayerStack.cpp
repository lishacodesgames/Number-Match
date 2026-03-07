#include <Precompiled.h>
#include <LayerStack.h>

#include <algorithm>
#include <Layer.h>
#include <vector>

void LayerStack::PushLayer(Layer* layer) {
   m_layers.emplace(begin() + m_layerInsertIndex, layer);
   m_layerInsertIndex++;
   
   layer->OnAttach();
}

void LayerStack::PushOverlay(Layer* overlay) {
   m_layers.emplace_back(overlay);
   overlay->OnAttach();
}

void LayerStack::PopLayer(Layer* layer) {
   std::vector<Layer*>::iterator it = 
      std::find(begin(), end(), layer);

   if(it != end()) {
      layer->OnDetach();
      m_layers.erase(it);
      m_layerInsertIndex--;
   }
}

std::vector<Layer*>::iterator LayerStack::begin() { return m_layers.begin(); }
std::vector<Layer*>::iterator LayerStack::end() { return m_layers.end(); }
std::vector<Layer*>::reverse_iterator LayerStack::rbegin() { return m_layers.rbegin(); }
std::vector<Layer*>::reverse_iterator LayerStack::rend() { return m_layers.rend(); }
bool LayerStack::empty() const { return m_layers.empty(); }

void LayerStack::Delete() {
   for(int i = 0; i < m_layers.size(); i++) {
      delete m_layers[i];
   }
   m_layers.clear();
   m_layerInsertIndex = 0;
}