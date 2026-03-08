#include <Precompiled.h>
#include "Button.h"

#include <raymath.h>
#include <raylib.h>
#include <utility>

Button::~Button() {
   if(IsTextureValid(icon))
      UnloadTexture(icon);
}

Button::Button(
   Rectangle exactBounds, 
   Texture2D* icon, const char* text, 
   Color buttonColor, Color contentColor,
   int fontSize, std::pair<float, int> roundness // default args
) 
   : m_bounds(exactBounds), roundness(roundness), text(text), fontSize(fontSize), buttonColor(buttonColor), contentColor(contentColor)
{
   origin = { m_bounds.x, m_bounds.y };
   if(icon)
      this->icon = *icon;

   Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 20, 1);
   float x = m_bounds.width / 2 - textSize.x / 2;
   float y = m_bounds.height / 2 - textSize.y / 2;
   
   m_horizontalPadding = { x, x };
   m_verticalPadding = { y,  y };
}

Button::Button(
   Vector2 origin, Vector2 padding, 
   Texture2D* icon, const char* text, 
   Color buttonColor, Color contentColor,
   int fontSize, std::pair<float, int> roundness // default args
) : origin(origin), roundness(roundness),text(text), fontSize(fontSize), buttonColor(buttonColor), contentColor(contentColor)
{ 
   if(icon)
      this->icon = *icon;
   setPadding_Bounds({padding.x, padding.x}, {padding.y, padding.y});
}

Button::Button (
   Vector2 origin, 
   float paddingLeft, float paddingRight, float paddingTop, float paddingBottom, 
   Texture2D* icon, const char* text, 
   Color buttonColor, Color contentColor,
   int fontSize, std::pair<float, int> roundness // default args
) : origin(origin), roundness(roundness), text(text), fontSize(fontSize), buttonColor(buttonColor), contentColor(contentColor)
{ 
   if(icon)
      this->icon = *icon;   
   setPadding_Bounds({paddingLeft, paddingRight}, {paddingTop, paddingBottom}); 
}

bool Button::isClicked() const {
   return (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), m_bounds));
}

void Button::setFocus(bool isFocused, Color buttonColor, Color contentColor) {
   this->isFocused = isFocused;
   this->buttonColor = buttonColor;
   this->contentColor = contentColor;
}

void Button::Update() {
  // update hover flag
   if (CheckCollisionPointRec(GetMousePosition(), m_bounds))
      isHovered = true;
   else
      isHovered = false;

  // update active flag
   if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && isHovered)
      isActive = true;
   else
      isActive = false;
}

void Button::Draw() {
   setPadding_Bounds(m_horizontalPadding, m_verticalPadding); // in case user has modified any values

   // draw button
   if(!isHovered)
      DrawRectangleRounded(m_bounds, roundness.first, roundness.second, buttonColor);
   else {
      if(buttonColor == WHITE) // darker for contrast cuz nothings lighter than white
         DrawRectangleRounded(
            m_bounds, roundness.first, roundness.second, ColorBrightness(buttonColor, -0.04232f)
         );
      else // lighter cuz it gives a satisfying pseudo-growth to button
         DrawRectangleRounded(
            m_bounds, roundness.first, roundness.second, ColorBrightness(buttonColor, 0.069f)
         );

      DrawRectangleRoundedLinesEx(
         m_bounds, roundness.first, roundness.second, 2, ColorBrightness(buttonColor, -0.12f)
      );
   }

   bool iconExists = IsTextureValid(icon);

   // make padding left be only on left and right only on right etc
   Vector2 contentSize = MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, 1);
   Vector2 padAfterIcon = {icon.width*1.5f, 0};
   if(iconExists) // add icon size and 5% more padding between icon and text
      contentSize += padAfterIcon*2;

   Vector2 contentOrigin;
   float availableHorizontalSpace = m_bounds.width - m_horizontalPadding.x - m_horizontalPadding.y - contentSize.x;
   contentOrigin.x = m_bounds.x + m_horizontalPadding.x + availableHorizontalSpace / 2;
   float availableVerticalSpace = m_bounds.height - m_verticalPadding.x -   m_verticalPadding.y - contentSize.y;
   contentOrigin.y = m_bounds.y + m_verticalPadding.x + availableVerticalSpace / 2;

   if(IsTextureValid(icon)) {
      DrawTexture(icon, contentOrigin.x, contentOrigin.y, contentColor);
      DrawTextEx(GetFontDefault(), text.c_str(), contentOrigin + padAfterIcon, fontSize, 1, contentColor);
   } else {
      DrawTextEx(GetFontDefault(), text.c_str(), contentOrigin, fontSize, 1, contentColor);
   }

}

void Button::setPadding_Bounds(Vector2 horizPadding, Vector2 vertPadding) {
   this->m_horizontalPadding = horizPadding;
   this->m_verticalPadding = vertPadding;

   m_bounds.x = origin.x;
   m_bounds.y = origin.y;

   Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, 1);
   m_bounds.width = textSize.x + horizPadding.x + horizPadding.y;
   m_bounds.height = textSize.y + vertPadding.x + vertPadding.y;
}

bool operator==(const Button& first, const Button& second) {
   return (
      first.origin == second.origin &&
      first.text == second.text &&
      first.fontSize == second.fontSize &&
      first.buttonColor  == second.buttonColor &&
      first.contentColor == second.contentColor &&
      first.m_bounds == second.m_bounds &&
      first.m_horizontalPadding == second.m_horizontalPadding &&
      first.m_verticalPadding == second.m_verticalPadding
   );
}
bool operator==(const Color& first, const Color& second) {
   return (
      first.r == second.r &&
      first.g == second.g &&
      first.b == second.b &&
      first.a == second.a
   );
}
bool operator==(const Rectangle& first, const Rectangle& second) {
   return (
      first.x == second.x &&
      first.y == second.y &&
      first.width == second.width &&
      first.height == second.height
   );
}