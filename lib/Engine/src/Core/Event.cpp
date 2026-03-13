#include <pch/Precompiled.h>
#include "Core/Event.h"

EventType Event::GetEventType() const { return EventType::None; }

KeyPressedEvent::KeyPressedEvent(char keycode) : key(keycode) {}
EventType KeyPressedEvent::GetEventType() const { return EventType::KeyPressed; }

MouseClickedEvent::MouseClickedEvent(bool isClickLeft) : isClickLeft(isClickLeft) {}
EventType MouseClickedEvent::GetEventType() const { return EventType::MouseClicked; }