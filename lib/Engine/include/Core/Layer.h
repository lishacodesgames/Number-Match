#pragma once
#include <string>
#include "Core/Event.h"

namespace Core {
   class Layer {
   public:
      /** @brief
       * @if true, then layer will always be pushed to the top of layerstack
       * @else layer will be push at the layer insert index (above layers but below overlays)
       */
      bool isOverlay = false;

      Layer( /// called when new layer pointer made with "new"
         const std::string& name, bool isOverlay = false, 
         bool renderSuspended = false, bool updateSuspended = false, bool eventSuspended = false
      );
      virtual ~Layer() = default; /// called with "delete" keyword

      // ---- Virtual functions with default implementations ----
      virtual void OnAttach(); /// called when layer is actually pushed onto layer stack
      virtual void OnDetach(); /// called when layer is actually popped from layer stack

      // ---- Pure virtual functions ----
      virtual void OnEvent(Event &e) = 0; /// check-once event (key press)
      virtual void OnUpdate() = 0; /// called every frame (key hold)
      virtual void OnRender() = 0;
      
      // ---- layer pause/resume (suspension) ----
      virtual void OnSuspend(bool render = false, bool update = false, bool event = false);
      virtual void OnResume();
      bool isSuspended = false;
      bool eventSuspended = false;
      bool updateSuspended = false;
      bool renderSuspended = false;

      // ---- Getters ----
      const std::string& GetName() const;

   protected:
      // ---- Protected Members ----
      std::string m_name;
   };
}