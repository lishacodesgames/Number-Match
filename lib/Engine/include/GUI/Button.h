#pragma once
#include <raylib.h>
#include <utility>
#include <string>

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
      // -----------------
      // ---- METHODS ----
      // -----------------

      void Update();
      void Draw();

      // ----------------------
      // ---- CONSTRUCTORS ----
      // ----------------------

      ~Button();

      /// Manual bounds settings, default text size, no padding, no roundness
      Button(
         Rectangle exactBounds, 
         const char* text, 
         Color buttonColor, Color contentColor,
         int fontSize = 20, std::pair<float, int> roundness = {0.8f, 8},
         Font font = GetFontDefault()
      );

      /// Evenly spaced padding
      Button(
         Vector2 origin, 
         Vector2 padding, 
         const char* text, 
         Color buttonColor, Color contentColor,
         int fontSize = 20, std::pair<float, int> roundness = {0.8f, 8},
         Font font = GetFontDefault()
      );

      /// Custom padding
      Button(
         Vector2 origin,
         float paddingLeft, float paddingRight, float paddingTop, float paddingBottom,
         const char* text, 
         Color buttonColor, Color contentColor,
         int fontSize = 20, std::pair<float, int> roundness = {0.8f, 8},
         Font font = GetFontDefault()
      );
      
      // ------------------------
      // ---- PUBLIC MEMBERS ----
      // ------------------------

      Texture icon = {0}; // optional
      std::string text; // compulsory

      Color buttonColor = BLACK, contentColor = WHITE;

      int fontSize = 20;
      std::pair<float, int> roundness = {0.8f, 8};

      Font font;

      // ---------------------
      // ---- GETS & SETS ----
      // ---------------------

      /// @param dimensions default = {0, 0}, keeps original dimensions of texture
      void setIcon(const char* filepath, Vector2 dimensions = {0, 0}); 
      void setIcon(Texture icon);
      
      void setOrigin(Vector2 origin);
      void setOrigin(int x, int y);

      void setSize(Vector2 size);
      void setBounds(Rectangle bounds);
      void setPadding(Vector2 horizPadding, Vector2 vertPadding);
      void setFocus(bool isFocused, Color buttonColor, Color contentColor);

      Vector2 getOrigin() const;
      Vector2 getSize() const;
      Rectangle getBounds() const;

      // ---------------
      // ---- FLAGS ----
      // ---------------

      bool isHovered = false; /// is button being hovered
      bool isActive = false; /// is button being clicked
      bool isFocused = false; /// set by user. Has button been clicked
      
   private: 
      // -------------------------
      // ---- PRIVATE MEMBERS ----
      // -------------------------

      Rectangle m_bounds;
      Vector2 m_horizontalPadding; /// {left, right}
      Vector2 m_verticalPadding;   /// {top, bottom}

      // -------------------
      // ---- OPERATORS ----
      // -------------------
      friend bool operator==(const Button& first, const Button& second);
   };
   
   bool operator==(const Button& first, const Button& second);
}

bool operator==(const Color& first, const Color& second);
bool operator==(const Rectangle& first, const Rectangle& second);