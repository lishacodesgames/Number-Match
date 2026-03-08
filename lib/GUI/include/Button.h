#pragma once
#include <raylib.h>
#include <utility>
#include <string>

class Button {
public:
   // ----------------------
   // ---- CONSTRUCTORS ----
   // ----------------------

   /** Constructor parameters' organisation
    * 
    *  -- compulsory --
    * Rectangle (Rectangle or origin+padding)
    * icon & text -- set nullptr and/or "" if not wanted
    * bg color & text+icon color
    * 
    * -- optional --
    * font size & button roundness
    * 
    */

   ~Button();

   /// Manual bounds settings, default text size, no padding, no roundness
   Button(
      Rectangle exactBounds, 
      Texture2D* icon, const char* text, 
      Color buttonColor, Color contentColor,
      int fontSize = 20, std::pair<float, int> roundness = {0.8f, 8}
   );

   /// Evenly spaced padding
   Button(
      Vector2 origin, 
      Vector2 padding, 
      Texture2D* icon, const char* text, 
      Color buttonColor, Color contentColor,
      int fontSize = 20, std::pair<float, int> roundness = {0.8f, 8}
   );

   // Custom padding
   Button(
      Vector2 origin,
      float paddingLeft, float paddingRight, float paddingTop, float paddingBottom,
      Texture2D* icon, const char* text, 
      Color buttonColor, Color contentColor,
      int fontSize = 20, std::pair<float, int> roundness = {0.8f, 8}
   );
   
   // ------------------------
   // ---- PUBLIC MEMBERS ----
   // ------------------------

   Vector2 origin = {0, 0};
   Texture2D icon = {0}; // optional
   std::string text; // compulsory
   int fontSize = 20;
   Color buttonColor = BLACK, contentColor = WHITE;
   std::pair<float, int> roundness = {0.8f, 8};

   // ---------------
   // ---- FLAGS ----
   // ---------------

   bool isHovered = false; // is button being hovered
   bool isActive = false; // is button being clicked
   bool isFocused = false; // set by user. Has button been clicked?

   // ----------------
   // ---- METHODS ----
   // ----------------

   bool isClicked() const; /// explicitly check at this moment is button being clicked
   void setFocus(bool isFocused, Color buttonColor, Color contentColor);
   void Update();
   void Draw();
   
private: 
   // -------------------------
   // ---- PRIVATE MEMBERS ----
   // -------------------------

   Rectangle m_bounds;
   Vector2 m_horizontalPadding; // {left, right}
   Vector2 m_verticalPadding;   // {top, bottom}

   // -----------------
   // ---- HELPERS ----
   // -----------------
   void setPadding_Bounds(Vector2 horizPadding, Vector2 vertPadding);

   // -------------------
   // ---- OPERATORS ----
   // -------------------
   friend bool operator==(const Button& first, const Button& second);
};

bool operator==(const Color& first, const Color& second);
bool operator==(const Rectangle& first, const Rectangle& second);