#pragma once

/// @todo resizing when suspended should be turned on regardless of which suspended flags are allowed (event/update/render)
/// @todo WindowResizeEvent. It should hold a function pointer for dispatching resizing

namespace Core
{
   enum class EventType {None /* should never be returned */, KeyPressed, MouseClicked};

   struct Event {
      bool Handled = false;
      virtual EventType GetEventType() const = 0;
   };

   struct KeyPressedEvent : Event {
      int key;

      KeyPressedEvent(int keycode);
      EventType GetEventType() const override;
   };

   struct MouseClickedEvent : Event {
      bool isClickLeft = true; /// false for right click, might implement other clicks later
      
      MouseClickedEvent(bool isClickLeft);
      EventType GetEventType() const override;
   };
}
