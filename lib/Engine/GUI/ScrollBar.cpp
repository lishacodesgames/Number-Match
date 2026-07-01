#include <pch/Precompiled.h>
#include "ScrollBar.h"

namespace GUI
{
   ScrollBar::ScrollBar(
      const Rectangle& track, float thumbWidth,
      Color thumbColor, Color trackColor) :
         thumbColor(thumbColor), trackColor(trackColor), m_track(track), m_thumb(track)
   { m_thumb.width = thumbWidth; }

   void ScrollBar::Update(float& scrollOffset, float contentHeight, float viewportHeight) {
      // Calculate the thumb's height based on the ratio of viewport to content
      float thumbHeight = std::max((viewportHeight / contentHeight) * m_track.height, m_minThumbHeight);
      float maxScrollOffset = contentHeight - viewportHeight;

      // Update thumb's position based on scroll offset
      scrollOffset = std::clamp(scrollOffset, 0.0f, maxScrollOffset);
      if(maxScrollOffset > 0) {
         float scrollRatio = scrollOffset / maxScrollOffset;
         m_thumb.y = m_track.y + scrollRatio * (m_track.height - thumbHeight);
      } else { // if content fits viewport, thumb sits in the middle of track, slightly smaller for aesthetically pleasing look
         thumbHeight = 0.985f * m_track.height;
         m_thumb.y = 1.035f * m_track.y;
      }

      m_thumb.height = thumbHeight;

      Vector2 mousePos = GetMousePosition();
      if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, m_thumb)) {
         m_isDragging = true;
         m_dragOffset = mousePos.y - m_thumb.y;
      } 

      if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
         m_isDragging = false;

      if(m_isDragging) {
         float newThumbY = mousePos.y - m_dragOffset; // where thumb SHOULD go
         float maxThumbY = m_track.y + m_track.height - m_thumb.height; // lowest thumb can go
         m_thumb.y = std::clamp(newThumbY, m_track.y, maxThumbY); // ensure thumb stays within track

         // Update scrollOffset based on thumb's new position
         float scrollRatio = (m_thumb.y - m_track.y) / (m_track.height - m_thumb.height);
         scrollOffset = scrollRatio * maxScrollOffset;
      }
   }

   void ScrollBar::Draw() const {
      DrawRectangleRec(m_track, trackColor);

      // being dragged makes it 20% darker
      DrawRectangleRec(m_thumb, m_isDragging ? ColorLerp(thumbColor, BLACK, 0.2f) : thumbColor);
   }
}