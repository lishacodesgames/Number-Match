#pragma once
#include <raylib.h>
#include "Roundness.h"

namespace GUI
{
   class Toggle {
   public:
      Color bgColor = LIGHTGRAY, knobColor = RED;

      Toggle() = default;
      Toggle(
         const Rectangle& bounds, Roundness roundness, float padding,
         Color bg = LIGHTGRAY, Color knob = RED
      );
   public:
      // -----------------
      // ---- SETTERS ----
      // -----------------

      void setOrigin(Vector2 origin) { m_bounds.x = origin.x; m_bounds.y = origin.y; }
      void setSize(Vector2 size);
      void setBounds(const Rectangle& bounds);

      void setRoundness(Roundness roundness);
      void setPadding(float padding);

      void setKnobIcon(const char* filepath, Vector2 dimensios = { 0, 0 });
   public:
      // -----------------
      // ---- GETTERS ----
      // -----------------
      Vector2 getOrigin() const { return { m_bounds.x, m_bounds.y }; }
      Vector2 getSize() const { return { m_bounds.width, m_bounds.height }; }
      Rectangle getBounds() const { return m_bounds; }

      Roundness getRoundness() const { return m_roundness; }
      float getPadding() const { return m_padding; }
      
      Texture getKnobIcon() const { return m_knobIcon; }
      float getKnobSize() const;
   private:
      Rectangle m_bounds;
      Roundness m_roundness;
      float m_padding = 0.0f;

      Texture m_knobIcon = {0};
   };
}