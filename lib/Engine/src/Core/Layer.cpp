#include <pch/Precompiled.h>
#include "Core/Layer.h"

#include "Core/Logging.h"

namespace Core {
   Layer::Layer(
      const std::string& name, bool isOverlay,
      bool suspended_render, bool suspended_update, bool suspended_event
   ) : m_name(name), isOverlay(isOverlay), suspended_render(suspended_render), suspended_update(suspended_update), suspended_event(suspended_event)
   {}

   void Layer::OnAttach() { TraceLog(LISHA_SAYS, "%s ATTACHED", m_name.c_str()); }
   void Layer::OnDetach() { TraceLog(LISHA_SAYS, "%s DETACHED", m_name.c_str()); }

   void Layer::OnSuspend(bool render, bool update, bool event) {
      isSuspended = true;
      suspended_render = render;
      suspended_update = update;
      suspended_event = event;

      TraceLog(LISHA_SAYS, "%s SUSPENDED", m_name.c_str()); 
   }
   
   void Layer::OnResume() {
      isSuspended = false;
      TraceLog(LISHA_SAYS, "%s RESUMED", m_name.c_str());
   }

   const std::string& Layer::GetName() const { return m_name; }
}