#include <Precompiled.h>
#include "Layer.h"

Layer::Layer(const std::string& name) : m_name(name) {}

void Layer::OnAttach() { TraceLog(LOG_INFO, "LISHA SAYS: %s ATTACHED", m_name.c_str()); }
void Layer::OnDetach() { TraceLog(LOG_INFO, "LISHA SAYS: %s DETACHED", m_name.c_str()); }

void Layer::OnSuspend() { TraceLog(LOG_INFO, "LISHA SAYS: %s SUSPENDED", m_name.c_str()); }
void Layer::OnResume() { TraceLog(LOG_INFO, "LISHA SAYS: %s RESUMED", m_name.c_str()); }