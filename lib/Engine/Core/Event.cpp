#include <pch/Precompiled.h>
#include "Event.h"

namespace Core
{
   KeyPressedEvent::KeyPressedEvent(char keycode) : key(keycode) {}
   EventType KeyPressedEvent::GetEventType() const { return EventType::KeyPressed; }

   MouseClickedEvent::MouseClickedEvent(bool isClickLeft) : isClickLeft(isClickLeft) {}
   EventType MouseClickedEvent::GetEventType() const { return EventType::MouseClicked; }

   WindowResizeEvent::WindowResizeEvent(int width, int height) : width(width), height(height) {}
   EventType WindowResizeEvent::GetEventType() const { return EventType::WindowResize; }
}