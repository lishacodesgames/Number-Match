#include <pch/Precompiled.h>
#include "GUI/ScrollBar.h"

void ScrollBar::Update(float scrollOffset, float contentHeight, float viewportHeight) {
   // Calculate the thumb's height based on the ratio of viewport to content
   float thumbHeight = std::max((viewportHeight / contentHeight) * m_track.height, m_minThumbHeight);

   // Update thumb's position based on scroll offset
   float maxScrollOffset = contentHeight - viewportHeight;
   scrollOffset = std::clamp(scrollOffset, 0.0f, maxScrollOffset); // ensure scrollOffset is within valid range
   if(maxScrollOffset > 0) {
      float scrollRatio = scrollOffset / maxScrollOffset;
      m_thumb.y = m_track.y + scrollRatio * (m_track.height - thumbHeight);
   } else { // if content fits viewport, thumb sits in the middle of track, slightly smaller for aesthetically pleasing look
      thumbHeight = 0.9f * m_track.height;
      m_thumb.y = 1.1f * m_track.y;
   }

   m_thumb.height = thumbHeight;
}

void ScrollBar::Draw() const {
   DrawRectangleRec(m_track, trackColor);
   DrawRectangleRec(m_thumb, thumbColor);
}