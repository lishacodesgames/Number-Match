# Making plans helps me think 
(AI IS BAD. HUMAN BRAINS FOR THE WIN!!!)

Also I like the markdown formatting. Good for brainstorming. (Take that you vibecoders :unamused:)

### The Buttons
- [x] Flags: Active = being clicked on, Focus = has been clicked on, Hover = being hovered on (based on CSS pseudo-classes)
- [x] Active checking functions for all 3 flags
- [x] Ensure functions are being checked/set per frame i.e. in OnUpdate() for whatever layer the button is in. FOR EACH BUTTON.
- [x] Remember to update the template's button GUI when this version is stable and bugless

### Menu Panel
I want the 3 buttons (home, daily, me) to be hyperlink type. <br>
**focus** & **active** = BLUE, **hover** = DARKBLUE, **inactive** = GRAY

Later, I will add Layers?Sublayers? for each page. For now, they are all MenuLayer, but they do need to adhere to the highlighting system of buttons.

- [x] Make sure 1 of the three buttons is in **focus** state at all times
- [x] If button1 == **focus** & button2 <- **active** then button1 <- **inactive**
- [x] Hover states of button does not change **focus**/**active** state of any other button

## Plan

### Buttons
- [x] Add Update() and the 3 flags to Button and make it a class
- [x] Remove the c stuff from template cuz class is now OOP so it's c++
- [x] Maybe add getBounds() to Button?
- [x] Add a setFocused() class to initalise Home to be focused first in MenuLayer

### MenuLayer
- [x] call Update() in MenuLayer's OnUpdate()
- [x] if (isActive) then setFocus true
- [x] Update OnEvent() and OnRender()