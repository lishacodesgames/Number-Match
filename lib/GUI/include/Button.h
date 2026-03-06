#pragma once
#include <raylib.h>

class Button {
public:
   // ----------------------
   // ---- CONSTRUCTORS ----
   // ----------------------

   Button() = default; // if user wants to set values manually

   /// Manual bounds settings, regargless of text size
   Button(Rectangle exactBounds, const char* text, Color buttonColor, Color textColor);

   /// Origin and padding settings, bounds are calculated based on default text size (20)
   Button(Vector2 origin, Vector2 padding, const char* text, Color buttonColor, Color textColor);

   /// Origin and padding settings, bounds are calculated based on custom text size
   Button(Vector2 origin, Vector2 padding, const char* text, int fontSize, Color buttonColor, Color textColor);

   /// Elaborate custom padding default text size (20)
   Button(
      Vector2 origin, 
      float paddingLeft, float paddingRight, 
      float paddingTop, float paddingBottom, 
      const char* text, 
      Color buttonColor, Color textColor
   );

   /// Elaborate custom padding custom text size
   Button(
      Vector2 origin, 
      float paddingLeft, float paddingRight, 
      float paddingTop, float paddingBottom, 
      const char* text, int fontSize, 
      Color buttonColor, Color textColor
   );
   
   // ------------------------
   // ---- PUBLIC MEMBERS ----
   // ------------------------

   Vector2 origin = {0, 0};
   const char* text;
   int fontSize = 20;
   Color buttonColor = BLACK, textColor = WHITE;

   // ---------------
   // ---- FLAGS ----
   // ---------------

   bool isHovered = false; // is button being hovered
   bool isActive = false; // is button being clicked
   bool isFocused = false; // set by user. Has button been clicked?

   // ----------------
   // ---- METHODS ----
   // ----------------

   void setFocus(bool isFocused, Color buttonColor, Color textColor);
   void Update();
   void Draw();
   
private: 
   // -------------------------
   // ---- PRIVATE MEMBERS ----
   // -------------------------

   Rectangle m_bounds;
   Vector2 m_horizontalPadding; // {left, right}
   Vector2 m_verticalPadding;   // {top, bottom}

   // -------------------
   // ---- OPERATORS ----
   // -------------------
   friend bool operator==(const Button& first, const Button& second);
};

bool operator==(const Color& first, const Color& second);
bool operator==(const Rectangle& first, const Rectangle& second);