#include <Precompiled.h>
#include <Layer.h>

Layer::Layer(const std::string& name) : m_name(name) {}

bool Layer::isSuspended() const { return m_isSuspended; }

void Layer::setSuspended(bool state) {
   if(m_isSuspended == state)
      return; // don't call suspension methods if state didn't change

   m_isSuspended = state;

   if(m_isSuspended)
      OnSuspend();
   else
      OnResume();
}
