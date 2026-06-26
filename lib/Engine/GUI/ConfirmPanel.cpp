#include <pch/Precompiled.h>
#include "ConfirmPanel.h"

namespace GUI
{
   ConfirmPanel::ConfirmPanel(
      Rectangle bounds, Roundness roundness, Color bgColor, std::string_view confirmationText, int fontSize,
      const Button& yesButton, const Button& noButton, Font font
   ) : yesButton(yesButton), noButton(noButton), bgColor(bgColor), roundness(roundness), m_bounds(bounds),
         m_confirmationText(confirmationText), m_fontSize(fontSize), m_font(font)
   { layoutContent(); }

   void ConfirmPanel::Update() {
      if(!isAsking)
         return;

      yesButton.Update();
      noButton.Update();
   }

   void ConfirmPanel::Draw() const {
      if(!isAsking)
         return;

      DrawRectangleRounded(m_bounds, roundness.roundness, roundness.segments, bgColor);

      const float spacing = 1.0f;
      const float lineGap = m_fontSize * 0.26f;
      float y = m_bounds.y + m_bounds.height * 0.22f;

      for(const std::string& line : m_lines) {
         Vector2 lineSize = MeasureTextEx(m_font, line.c_str(), m_fontSize, spacing);
         DrawTextEx(
            m_font, line.c_str(),
            { m_bounds.x + (m_bounds.width - lineSize.x) / 2.0f, y },
            m_fontSize, spacing, noButton.contentColor);
         y += m_fontSize + lineGap;
      }

      yesButton.Draw();
      noButton.Draw();
   }

   bool ConfirmPanel::Ask() {
      isAsking = true;
      Update();
      return yesButton.isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
   }

   void ConfirmPanel::setBounds(Vector2 bounds, bool resizeContent) {
      Vector2 oldSize = { m_bounds.width, m_bounds.height };
      m_bounds.width = bounds.x;
      m_bounds.height = bounds.y;

      if(!resizeContent) {
         layoutContent();
         return;
      }

      float widthScale = m_bounds.width / oldSize.x;
      float heightScale = m_bounds.height / oldSize.y;

      m_fontSize *= heightScale;

      yesButton.setBounds({ yesButton.getSize().x * widthScale, yesButton.getSize().y * heightScale }, true);
      noButton.setBounds({ noButton.getSize().x * widthScale, noButton.getSize().y * heightScale }, true);
      layoutContent();
   }

   void ConfirmPanel::layoutContent() {
      float padding = m_bounds.width * 0.09f;
      float maxTextWidth = m_bounds.width - padding * 2.0f;
      m_lines = wrapText(maxTextWidth);

      float buttonWidth = (m_bounds.width - padding * 2.0f - m_bounds.width * 0.055f) / 2.0f;
      float buttonHeight = std::max(m_bounds.height * 0.18f, (float)m_fontSize * 1.8f);
      float buttonY = m_bounds.y + m_bounds.height - padding - buttonHeight;

      int fontSize = std::max(1, (int)(buttonHeight * 0.44f));
      yesButton.setFontSize(fontSize);
      noButton.setFontSize(yesButton.getFontSize());
      yesButton.setBounds({ buttonWidth, buttonHeight }, false);
      noButton.setBounds({ buttonWidth, buttonHeight }, false);

      yesButton.setOrigin({ m_bounds.x + padding, buttonY });
      noButton.setOrigin({ m_bounds.x + m_bounds.width - padding - buttonWidth, buttonY });
   }

   /// @todo account for \n characters
   std::vector<std::string> ConfirmPanel::wrapText(float maxWidth) const {
      std::vector<std::string> lines;
      std::istringstream words(m_confirmationText);
      std::string word, line;

      while(words >> word) {
         std::string candidate = line.empty() ? word : line + " " + word;
         if(MeasureTextEx(m_font, candidate.c_str(), m_fontSize, 1.0f).x <= maxWidth) {
            line = candidate;
            continue;
         }

         if(!line.empty())
            lines.push_back(line);
         line = word;
      }

      if(!line.empty())
         lines.push_back(line);

      if(lines.empty())
         lines.push_back("");

      return lines;
   }
}
