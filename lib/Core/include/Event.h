#pragma once

enum class EventType {None, KeyPressed, MouseClicked};

struct Event {
   bool Handled = false;
   virtual EventType GetEventType() const;
};

struct KeyPressedEvent : Event {
   char key;

   KeyPressedEvent(char keycode);
   EventType GetEventType() const override;
};

struct MouseClickedEvent : Event {
   bool isClickLeft = true;
   
   MouseClickedEvent(bool isClickLeft);
   EventType GetEventType() const override;
};