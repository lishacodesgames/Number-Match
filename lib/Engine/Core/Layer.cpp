#include <pch/Precompiled.h>
#include "Layer.h"

namespace Core
{
   Layer::Layer(
      const std::string& name, bool isOverlay,
      bool renderSuspended, bool updateSuspended, bool eventSuspended) :
         m_name(name), isOverlay(isOverlay), renderSuspended(renderSuspended),
         updateSuspended(updateSuspended), eventSuspended(eventSuspended)
   {}

   void Layer::OnAttach() { LOG_LAYER("{} ATTACHED", m_name); }
   void Layer::OnDetach() { LOG_LAYER("{} DETACHED", m_name); }

   void Layer::OnSuspend(bool render, bool update, bool event) {
      isSuspended = true;
      renderSuspended = render;
      updateSuspended = update;
      eventSuspended = event;

      LOG_LAYER("{} SUSPENDED", m_name); 
   }
   
   void Layer::OnResume() {
      if(!isSuspended)
         Core::ConsoleLog(LOG_WARNING, std::format("Tried to resume {} but it wasn't suspended!", m_name));
      isSuspended = false;
      LOG_LAYER("{} RESUMED", m_name);
   }
}