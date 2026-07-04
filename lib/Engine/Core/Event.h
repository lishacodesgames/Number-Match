#pragma once

namespace Core
{
   enum class EventType {None /* should never be returned */, KeyPressed, MouseClicked, WindowResize};

   struct Event {
      bool Handled = false;
      virtual EventType GetEventType() const = 0;
   };

   struct KeyPressedEvent : Event {
      char key;

      KeyPressedEvent(char keycode);
      EventType GetEventType() const override;
   };

   struct MouseClickedEvent : Event {
      bool isClickLeft = true; /// false for right click, might implement other clicks later
      
      MouseClickedEvent(bool isClickLeft);
      EventType GetEventType() const override;
   };

   struct WindowResizeEvent : Event {
      int width, height;

      WindowResizeEvent(int width, int height);
      EventType GetEventType() const override;
   };
}
