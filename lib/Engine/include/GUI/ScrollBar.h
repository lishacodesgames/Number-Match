#pragma once
#include <raylib.h>

/// Only visual, no mouse dragging. Can only scroll vertically.
class ScrollBar {
public:
   Color thumbColor = GRAY, trackColor = LIGHTGRAY;

   ScrollBar() = default;
   ScrollBar(const Rectangle& track, float thumbWidth, Color thumbColor = GRAY, Color trackColor = LIGHTGRAY)
      : thumbColor(thumbColor), trackColor(trackColor), m_track(track), m_thumb(track)  // initally, thumb and track are same
   { m_thumb.width = thumbWidth; }

   /** parameters
    * @param scrollOffset thumb's position on track
    * @param contentHeight total height of scrollable region
    * @param viewportHeight visible height, used to calculate thumb's size relative to track
    */
   void Update(float& scrollOffset, float contentHeight, float viewportHeight);
   void Draw() const;

   inline void setMinThumbHeight(float height) { m_minThumbHeight = height; } /// default = 20.0f
   inline void setThumbWidth(float width) { m_thumb.width = width; } /// height is set according to Update params
   inline void setTrackBounds(const Rectangle& track) { m_track = track; m_thumb.x = track.x + (track.width - m_thumb.width) / 2; }
private:
   Rectangle m_track{}; // the path
   Rectangle m_thumb{}; // the movable piece

   float m_minThumbHeight = 20.0f;

   bool m_isDragging = false;
   float m_dragOffset = 0.0f;
};