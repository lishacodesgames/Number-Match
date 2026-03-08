#include <Precompiled.h>
#include "Layer.h"

Layer::Layer(const std::string& name, bool isOverlay) : m_name(name), isOverlay(isOverlay) {}

void Layer::OnAttach() { TraceLog(LOG_INFO, "LISHA SAYS: %s ATTACHED", m_name.c_str()); }
void Layer::OnDetach() { TraceLog(LOG_INFO, "LISHA SAYS: %s DETACHED", m_name.c_str()); }

void Layer::OnSuspend() { TraceLog(LOG_INFO, "LISHA SAYS: %s SUSPENDED", m_name.c_str()); }
void Layer::OnResume() { TraceLog(LOG_INFO, "LISHA SAYS: %s RESUMED", m_name.c_str()); }