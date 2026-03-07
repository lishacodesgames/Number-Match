#pragma once
#include <Event.h>
#include <string>

class Layer {
public:
   Layer(const std::string& name); /// called when new layer pointer made with "new"
   virtual ~Layer() = default; /// called with "delete" keyword

   // ---- Pure virtual functions ----
   virtual void OnAttach() = 0; /// called when layer is actually pushed onto layer stack
   virtual void OnDetach() = 0; /// called when layer is actually popped from layer stack
   virtual void OnUpdate() = 0; /// called every frame (key hold)
   virtual void OnEvent(Event &e) = 0; /// check-once event (key press)
   virtual void OnRender() = 0;

   // ---- layer pause/resume (suspension) ----
   virtual void OnSuspend() {};
   virtual void OnResume() {};

   bool isSuspended() const;
   void setSuspended(bool state);
protected:
   std::string m_name;
   bool m_isSuspended = false;
};