#include <pch/Precompiled.h>
#include "Event.h"

namespace Core
{
   KeyPressedEvent::KeyPressedEvent(int keycode) : key(keycode) {}
   EventType KeyPressedEvent::GetEventType() const { return EventType::KeyPressed; }

   MouseClickedEvent::MouseClickedEvent(bool isClickLeft) : isClickLeft(isClickLeft) {}
   EventType MouseClickedEvent::GetEventType() const { return EventType::MouseClicked; }
}
