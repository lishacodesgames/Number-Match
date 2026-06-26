#pragma once
#include <raylib.h>
#include <utility>
#include <string>
#include "Roundness.h"

namespace GUI {
   /** Constructor parameters' organisation
    *
    *  -- compulsory --
    * Bounds (Rectangle or origin+padding)
    * text -- set "" if not wanted
    * bg color & text+icon color
    *
    * -- optional --
    * font size & button roundness
    * custom font
    *
    */
   class Button {
   public:
      void Update();
      void Draw() const;
   public:
      // ----------------------
      // ---- CONSTRUCTORS ----
      // ----------------------

      ~Button();

      /// Manual bounds settings, default text size, no padding, no roundness
      Button(
         Rectangle exactBounds, 
         const char* text, 
         Color bgColor, Color contentColor,
         int fontSize = 20, Roundness roundness = {0.8f, 8},
         Font font = GetFontDefault()
      );

      /// Evenly spaced padding
      Button(
         Vector2 origin, 
         Vector2 padding, 
         const char* text, 
         Color bgColor, Color contentColor,
         int fontSize = 20, Roundness roundness = {0.8f, 8},
         Font font = GetFontDefault()
      );

      /// Custom padding
      Button(
         Vector2 origin,
         Vector2 horizPadding, Vector2 vertPadding,
         const char* text, 
         Color bgColor, Color contentColor,
         int fontSize = 20, Roundness roundness = {0.8f, 8},
         Font font = GetFontDefault()
      );

   public:
      // -----------------
      // ---- SETTERS ----
      // -----------------

      /// @param dimensions default = {0, 0}, keeps original dimensions
      void setIcon(const char* filepath, Vector2 dimensions = { 0, 0 });
      void setText(const std::string& text) { m_text = text; recalculateLayout(); }

      void setFont(Font font) { m_font = font; recalculateLayout(); }
      void setFontSize(int fontSize);
      void setRoundness(Roundness roundness) { m_roundness = roundness; }

      /// @param resizeContent TRUE: will resize contents to stay in same ratio as before -- FALSE: will resize only rectangle
      void setBounds(Vector2 bounds, bool resizeContent);
      void setOrigin(Vector2 origin) { m_bounds.x = origin.x; m_bounds.y = origin.y; }
      void setOrigin(int x, int y) { m_bounds.x = x; m_bounds.y = y; }

      /// horizontal = {left, right}, vertical = {top, bottom}
      void setPadding(Vector2 horizPadding, Vector2 vertPadding);
      /// horizontal = left & right padding, vertical = top & bottom
      void setPadding(float horizPadding, float vertPadding) {
         setPadding({ horizPadding, horizPadding }, { vertPadding, vertPadding });
      }
      /// even padding
      void setPadding(float padding) { setPadding({ padding, padding }, { padding, padding }); }

      void setFocus(bool isFocused, Color bgColor, Color contentColor);
   public:
      // -----------------
      // ---- GETTERS ----
      // -----------------
      Texture getIcon() const { return m_iconTexture; }
      std::string getText() const { return m_text; }

      Font getFont() const { return m_font; }
      int getFontSize() const { return m_fontSize; }
      Roundness getRoundness() const { return m_roundness; }
         
      Rectangle getBounds() const { return m_bounds; }
      Vector2 getOrigin() const { return { m_bounds.x, m_bounds.y }; }           /// @return origin of the button's bounds
      Vector2 getSize() const { return { m_bounds.width, m_bounds.height }; }    /// @return size of button's bounds
      Vector2 getHorizontalPadding() const { return m_horizontalPadding; }
      Vector2 getVerticalPadding() const { return m_verticalPadding; }

      Vector2 getIconOrigin() const;
      Vector2 getTextOrigin() const;
      
   public:
      // ---------------
      // ---- FLAGS ----
      // ---------------

      bool isHovered = false;  /// is button being hovered
      bool isActive = false;   /// is button being clicked
      bool isFocused = false;  /// set by user. Has button been clicked

   private:
      // -----------------
      // ---- MEMBERS ----
      // -----------------

      Image m_iconImage = { 0 };
      Texture m_iconTexture = { 0 };   // optional
      std::string m_text;              // compulsory

      Font m_font;
      int m_fontSize = 20;
      Roundness m_roundness = { 0.8f, 8 };

      Rectangle m_bounds;
      Vector2 m_horizontalPadding;  /// {left, right}
      Vector2 m_verticalPadding;    /// {top, bottom}
   public:
      Color bgColor = BLACK, contentColor = WHITE;
   private:
      // -----------------
      // ---- HELPERS ----
      // -----------------
      void recalculateLayout();
      friend bool operator==(const Button& first, const Button& second);
   };
}  // namespace GUI

bool operator==(const Color& first, const Color& second);
bool operator==(const Rectangle& first, const Rectangle& second);