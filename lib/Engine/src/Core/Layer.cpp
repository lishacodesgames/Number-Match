#include <pch/Precompiled.h>
#include "Core/Layer.h"

#include "Core/Logging.h"

namespace Core {
   Layer::Layer(
      const std::string& name, bool isOverlay,
      bool renderSuspended, bool updateSuspended, bool eventSuspended
   ) : m_name(name), isOverlay(isOverlay), renderSuspended(renderSuspended), updateSuspended(updateSuspended), eventSuspended(eventSuspended)
   {}

   void Layer::OnAttach() { TraceLog(LISHA_SAYS, "%s ATTACHED", m_name.c_str()); }
   void Layer::OnDetach() { TraceLog(LISHA_SAYS, "%s DETACHED", m_name.c_str()); }

   void Layer::OnSuspend(bool render, bool update, bool event) {
      isSuspended = true;
      renderSuspended = render;
      updateSuspended = update;
      eventSuspended = event;

      TraceLog(LISHA_SAYS, "%s SUSPENDED", m_name.c_str()); 
   }
   
   void Layer::OnResume() {
      isSuspended = false;
      TraceLog(LISHA_SAYS, "%s RESUMED", m_name.c_str());
   }

   const std::string& Layer::GetName() const { return m_name; }
}