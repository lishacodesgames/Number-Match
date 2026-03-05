#pragma once
#include <Event.h>
#include <string>

class Layer {
public:
   Layer(const std::string& name);
   virtual ~Layer() = default;

   virtual void OnAttach() = 0;
   virtual void OnDetach() = 0;
   virtual void OnUpdate() = 0; /// called every frame (key hold)
   virtual void OnEvent(Event &e) = 0; /// check-once event (key press)
   virtual void OnRender() = 0;

protected:
   std::string m_name;
};