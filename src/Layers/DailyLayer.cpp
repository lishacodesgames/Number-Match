#include <pch/Precompiled.h>
#include "DailyLayer.h"

#include "Storage.h"
#include "Colors.h"

void DailyLayer::OnEvent(Core::Event& e) {
   if(e.GetEventType() == Core::EventType::KeyPressed)
      Core::ConsoleLog(LISHA_SAYS, std::format("The {} key was pressed", static_cast<Core::KeyPressedEvent&>(e).key));
   else if(e.GetEventType() == Core::EventType::MouseClicked)
      Core::ConsoleLog(LISHA_SAYS, "Mouse was clicked");
}

void DailyLayer::OnUpdate() {}

void DailyLayer::OnRender() {
   DrawTextEx(
      Storage::ui.font_semibold, "Coming soon...",
      { GetScreenWidth() / 2.0f - 150, GetScreenHeight() / 2.0f - 100 },
      35, 2.5f, Palette::text_for_off_bright);
}