#include <pch/Precompiled.h>
#include "QuestionLayer.h"

#include "Colors.h"
#include "App.h"

#define INIT_BUTTON_TEXT 20, { 0.8f, 8 }, App::font_semibold
#define NO_REC { 0, 0, 0, 0 }

QuestionLayer::QuestionLayer(std::string_view question, std::function<void(bool)> onAnswer, Core::Layer* caller)
      : Core::Layer("Question Layer", true),
      m_panel(
         NO_REC, { 0.16f, 8 }, Palette::bright_bg,
         question, 24,
         GUI::Button(NO_REC, "Yes", Palette::home_button_2, Palette::home_button_1, INIT_BUTTON_TEXT),
         GUI::Button(NO_REC, "No", Palette::home_button_1, Palette::home_button_2, INIT_BUTTON_TEXT),
         App::font_semibold),
      m_onAnswer(std::move(onAnswer)),
      m_caller(caller)
{
   resize();
   m_panel.isAsking = true;
}

void QuestionLayer::OnAttach() {
   if(m_caller)
      m_caller->OnSuspend(true);

   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
   Core::Layer::OnAttach();
}

void QuestionLayer::OnEvent(Core::Event& e) {
   e.Handled = true;

   if(e.GetEventType() == Core::EventType::MouseClicked) {
      if(m_panel.yesButton.isHovered)
         answer(true);
      else if(m_panel.noButton.isHovered)
         answer(false);
      else
         e.Handled = false;
   } else if(e.GetEventType() == Core::EventType::KeyPressed) {
      int key = static_cast<Core::KeyPressedEvent&>(e).key;
      if(key == 'y' || key == 'Y' || key == KEY_ENTER)
         answer(true);
      else if(key == 'n' || key == 'N' || key == KEY_ESCAPE)
         answer(false);
      else
         e.Handled = false;
   } else if(e.GetEventType() == Core::EventType::WindowResize)
      resize();
}

void QuestionLayer::OnUpdate() {
   m_panel.Update();

   if(m_panel.yesButton.isHovered || m_panel.noButton.isHovered)
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void QuestionLayer::OnRender() {
   DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Palette::darken_game_bg);
   m_panel.Draw();
}

void QuestionLayer::answer(bool yes) {
   if(m_answered)
      return;

   m_answered = true;
   m_onAnswer(yes);
   if(m_caller && App::GetLayerByName(m_caller->GetName()))
      m_caller->OnResume();

   App::QueueLayerPop(this);
}

void QuestionLayer::resize() {
   float width = std::clamp(GetScreenWidth() * 0.78f, 320.0f, 520.0f);
   float maxHeight = std::max(GetScreenHeight() * 0.48f, 190.0f);
   float height = std::clamp(width * 0.52f, 190.0f, maxHeight);

   m_panel.setBounds({ width, height }, false);
   m_panel.setOrigin({
      (GetScreenWidth() - width) / 2.0f,
      (GetScreenHeight() - height) / 2.0f
   });
   m_panel.setFontSize(std::max<int>(20, height * 0.13f));
}