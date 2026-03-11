#pragma once
#include "Button.h"
#include "Event.h"
#include "Layer.h"

#include <raylib.h>

class OptionsLayer : public Layer {
public:
   OptionsLayer();
   ~OptionsLayer() override = default;

   void OnAttach() override;
   void OnEvent(Event& e) override;
   void OnUpdate() override;
   void OnRender() override;
private:
   Rectangle m_bounds;
   Button m_doneButton;
};