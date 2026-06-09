#include <pch/Precompiled.h>
#include "Core/Layer.h"

namespace Core {

   Layer::Layer(
      const std::string& name, bool isOverlay,
      bool renderSuspended, bool updateSuspended, bool eventSuspended
   ) : m_name(name), isOverlay(isOverlay), renderSuspended(renderSuspended), updateSuspended(updateSuspended), eventSuspended(eventSuspended) {}

   void Layer::OnAttach() { LOG_LAYER("%s ATTACHED", m_name.c_str()); }
   void Layer::OnDetach() { LOG_LAYER("%s DETACHED", m_name.c_str()); }

   void Layer::OnSuspend(bool render, bool update, bool event) {
      isSuspended = true;
      renderSuspended = render;
      updateSuspended = update;
      eventSuspended = event;

      LOG_LAYER("%s SUSPENDED", m_name.c_str()); 
   }
   
   void Layer::OnResume() {
      isSuspended = false;
      LOG_LAYER("%s RESUMED", m_name.c_str());
   }

   const std::string& Layer::GetName() const { return m_name; }
}