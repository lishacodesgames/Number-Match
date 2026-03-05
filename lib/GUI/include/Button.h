#pragma once
#include <raylib.h>

struct Button {
   Button() = default; // if user wants to set values manually

   /// Manual bounds settings, regargless of text size
   Button(Rectangle exactBounds, const char* text, Color buttonColor, Color textColor);

   /// Origin and padding settings, bounds are calculated based on text size (default text size is 20)
   Button(Vector2 origin, Vector2 padding, const char* text, Color buttonColor, Color textColor);

   /// Origin and padding settings, bounds are calculated based on text size, with custom text size
   Button(Vector2 origin, Vector2 padding, const char* text, int fontSize, Color buttonColor, Color textColor);

   /// More custom padding (default text size = 20)
   Button(
      Vector2 origin, 
      float paddingLeft, float paddingRight, 
      float paddingTop, float paddingBottom, 
      const char* text, 
      Color buttonColor, Color textColor
   );

   /// More custom padding (custom text size)
   Button(
      Vector2 origin, 
      float paddingLeft, float paddingRight, 
      float paddingTop, float paddingBottom, 
      const char* text, int fontSize, 
      Color buttonColor, Color textColor
   );

   Vector2 origin;
   const char* text;
   int fontSize = 20;
   Color buttonColor, textColor;
   
   bool isHovering();
   bool isClicked();
   void Draw();
   
private:
   Rectangle m_bounds;
   Vector2 m_horizontalPadding; // {left, right}
   Vector2 m_verticalPadding;   // {top, bottom}
};