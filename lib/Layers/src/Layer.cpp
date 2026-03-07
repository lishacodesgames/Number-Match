#include <Precompiled.h>
#include <Layer.h>

#include <cstdio>

Layer::Layer(const std::string& name) : m_name(name) {}

void Layer::OnAttach() { printf("%s ATTACHED", m_name.c_str()); }
void Layer::OnDetach() { printf("%s DETACHED", m_name.c_str()); }