#pragma once
#include <string>
#include "Event.h"

class Layer {
public:
   Layer(const std::string& name); /// called when new layer pointer made with "new"
   virtual ~Layer() = default; /// called with "delete" keyword

   // ---- Pure virtual functions ----
   virtual void OnAttach(); /// called when layer is actually pushed onto layer stack
   virtual void OnDetach(); /// called when layer is actually popped from layer stack
   virtual void OnUpdate() = 0; /// called every frame (key hold)
   virtual void OnEvent(Event &e) = 0; /// check-once event (key press)
   virtual void OnRender() = 0;

   // ---- layer pause/resume (suspension) ----
   bool renderSuspended = false;
   virtual void OnSuspend();
   virtual void OnResume();
protected:
   std::string m_name;
};