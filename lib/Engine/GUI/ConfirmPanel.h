#pragma once
#include "Button.h"

namespace GUI
{
   class ConfirmPanel {
   public:
      Button yesButton, noButton;
      Color bgColor;

      Roundness roundness;
      bool isAsking = false;

   public:
      ConfirmPanel(
         Rectangle bounds, Roundness roundness, Color bgColor,
         std::string_view confirmationText, int fontSize,
         const Button& yesButton, const Button& noButton,
         Font font = GetFontDefault());
      ~ConfirmPanel() = default;

      void Update();
      void Draw() const; /// only renders if isAsking

      bool Ask(); /// shows panel, returns user response
   
   public:
      // -----------------
      // ---- SETTERS ----
      // -----------------

      void setBounds(Vector2 bounds, bool resizeContent);
      void setOrigin(Vector2 origin) {
         m_bounds.x = origin.x; m_bounds.y = origin.y;
         layoutContent();
      }

      void setText(std::string_view text) { m_confirmationText = text; layoutContent(); }
      void setFont(Font font) { m_font = font; layoutContent(); }
      void setFontSize(int fontSize) { m_fontSize = fontSize; layoutContent(); }

   public:
      // -----------------
      // ---- GETTERS ----
      // -----------------

      Rectangle getBounds() const { return m_bounds; }
      Vector2 getSize() const { return { m_bounds.width, m_bounds.height }; }
      Vector2 getOrigin() const { return { m_bounds.x, m_bounds.y }; }

      std::string getText() const { return m_confirmationText; }
      int getFontSize() const { return m_fontSize; }

   private:
      Rectangle m_bounds;

      std::string m_confirmationText;
      int m_fontSize;
      Font m_font;

      std::vector<std::string> m_lines;

   private:
      void layoutContent();
      std::vector<std::string> wrapText(float maxWidth) const;
   };
}
