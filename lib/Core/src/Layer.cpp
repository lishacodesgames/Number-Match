#include <Precompiled.h>
#include "Layer.h"

Layer::Layer(
   const std::string& name, bool isOverlay,
   bool suspended_render, bool suspended_update, bool suspended_event
) : m_name(name), isOverlay(isOverlay), suspended_render(suspended_render), suspended_update(suspended_update), suspended_event(suspended_event)
{}

void Layer::OnAttach() { TraceLog(LOG_INFO, "LISHA SAYS: %s ATTACHED", m_name.c_str()); }
void Layer::OnDetach() { TraceLog(LOG_INFO, "LISHA SAYS: %s DETACHED", m_name.c_str()); }

void Layer::OnSuspend() {
   isSuspended = true;
   TraceLog(LOG_INFO, "LISHA SAYS: %s SUSPENDED", m_name.c_str()); 
}
void Layer::OnResume() {
   isSuspended = false;
   TraceLog(LOG_INFO, "LISHA SAYS: %s RESUMED", m_name.c_str());
}

const std::string& Layer::GetName() { return m_name; }