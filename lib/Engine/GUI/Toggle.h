#pragma once
#include <raylib.h>
#include "Roundness.h"

namespace GUI {
   /// @todo add separate (almost)everything based on state
   /// @todo add dragability (maybe make a flag for it? m_draggable)
   class Toggle {
   public:
      Color bgColor = LIGHTGRAY, knobColor = RED;
      Roundness roundness;
      bool isHovered;  /// true IF knob is hovered

      void Update();
      void Draw() const;

      ~Toggle() {
         if(IsTextureValid(m_knobTexture))
            UnloadTexture(m_knobTexture);
         if(IsImageValid(m_knobImage))
            UnloadImage(m_knobImage);
      }

      Toggle() = default;
      Toggle(
         const Rectangle& bounds, Roundness roundness, float padding,
         Color bg = LIGHTGRAY, Color knob = RED
      ) : bgColor(bg), knobColor(knob), roundness(roundness), m_bounds(bounds), m_padding(padding)
         { setKnob(); }

   public:
      // -----------------
      // ---- SETTERS ----
      // -----------------

      void setOrigin(Vector2 origin) {
         m_bounds.x = origin.x;
         m_bounds.y = origin.y;
      }

      void setSize(Vector2 size) {
         m_bounds.width = size.x;
         m_bounds.height = size.y;
         setKnob();
      }

      void setBounds(const Rectangle& bounds) {
         m_bounds = bounds;
         setKnob();
      }

      /// @todo add animation
      void setState(bool state) {
         m_state = state;
         setKnob();
      }

      void setPadding(float padding) {
         m_padding = padding;
         setKnob();
      }

      /// @param padding the padding INSIDE knob AROUND texture
      void setKnobIcon(const char* filepath, float padding);
   private:
      void setKnob();

   public:
      // -----------------
      // ---- GETTERS ----
      // -----------------
      Vector2 getOrigin() const { return { m_bounds.x, m_bounds.y }; }
      Vector2 getSize() const { return { m_bounds.width, m_bounds.height }; }
      Rectangle getBounds() const { return m_bounds; }

      bool getState() const { return m_state; }  /// false = left, true = right
      float getPadding() const { return m_padding; }

      Texture getKnobIcon() const { return m_knobTexture; }
      Vector2 getKnobSize() const {
         return { m_bounds.width / 2 - m_padding * 2, m_bounds.height - m_padding * 2 };
      }

   private:
      Rectangle m_bounds, m_knob{};

      bool m_state = false;
      float m_padding = 0.0f;

      Image m_knobImage = { 0 };
      Texture m_knobTexture = { 0 };
   };
}  // namespace GUI

inline Vector2 operator-(Vector2 vec, float fl) { return { vec.x - fl, vec.y - fl }; }