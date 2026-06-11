#include <pch/Precompiled.h>
#include "Toggle.h"

namespace GUI
{
#pragma region Methods
   void Toggle::Update() {
      // update hover flag
      if(CheckCollisionPointRec(GetMousePosition(), m_knob))
         isHovered = true;
      else
         isHovered = false;

      // update active flag
      if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && isHovered)
         m_state = !m_state;
   }

   void Toggle::Draw() const {
      DrawRectangleRounded(m_bounds, roundness.roundness, roundness.segments, bgColor);
      DrawRectangleRounded(m_knob, roundness.roundness, roundness.segments, knobColor);

      if(IsTextureValid(m_knobTexture)) {
         DrawTexture(m_knobTexture,
            m_knob.x + (m_knob.width - m_knobTexture.width) / 2,
            m_knob.y + (m_knob.height - m_knobTexture.height) / 2,
            WHITE
         );
      }
   }
#pragma endregion

#pragma region Setters
   void Toggle::setKnobIcon(const char* filepath, float padding) {
      m_knobImage = LoadImage(filepath);
      float aspect = (float)m_knobImage.width / m_knobImage.height;

      Vector2 knobSize = getKnobSize();
      Vector2 size;
      if(knobSize.x <= knobSize.y) {
         size.x = knobSize.x - padding * 2;
         size.y = std::min(size.x / aspect, knobSize.y - padding * 2);
      } else {
         size.y = knobSize.y - padding * 2;
         size.x = std::min(size.y * aspect, knobSize.x - padding * 2);
      }

      if(size != Vector2{ (float)m_knobImage.width, (float)m_knobImage.height }) {
         Image resize = ImageCopy(m_knobImage);
         ImageResize(&resize, size.x, size.y);
         m_knobTexture = LoadTextureFromImage(resize);
      } else {
         m_knobTexture = LoadTextureFromImage(m_knobImage);
      }
   }

   void Toggle::setKnob() {
      m_knob = {
         m_bounds.x + m_padding + (m_state ? m_bounds.width / 2 : 0),
         m_bounds.y + m_padding, getKnobSize().x, getKnobSize().y
      };
   }
#pragma endregion
}  // namespace GUI