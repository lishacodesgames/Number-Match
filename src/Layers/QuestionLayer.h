#pragma once
#include "Core/Layer.h"
#include "GUI/ConfirmPanel.h"

/// @bug When clicking home immediately after closing QuestionLayer, clicking Me/Daily doesn't unfocus homeButton
/// and clicking it doesn't take us back to home
class QuestionLayer : public Core::Layer {
public:
   QuestionLayer(
      std::string_view question,
      std::function<void(bool)> onAnswer,
      Core::Layer* caller = nullptr);
   ~QuestionLayer() override = default;

   void OnAttach() override;

   void OnEvent(Core::Event& e) override;
   void OnUpdate() override;
   void OnRender() override;

private:
   GUI::ConfirmPanel m_panel;
   std::function<void(bool)> m_onAnswer;
   Core::Layer* m_caller;
   bool m_answered = false;

private:
   void answer(bool yes);
   void resize();
};
