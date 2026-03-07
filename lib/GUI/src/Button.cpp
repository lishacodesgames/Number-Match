#include <Precompiled.h>
#include <raymath.h>
#include <raylib.h>
#include <Button.h>
#include <utility>

Button::Button(Rectangle exactBounds, const char* text, Color buttonColor, Color textColor) 
   : m_bounds(exactBounds), text(text), buttonColor(buttonColor), textColor(textColor)
{
   origin = { m_bounds.x, m_bounds.y };

   Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 20, 1);
   float x = m_bounds.width / 2 - textSize.x / 2;
   float y = m_bounds.height / 2 - textSize.y / 2;
   
   m_horizontalPadding = { x, x };
   m_verticalPadding = { y,  y };
}

Button::Button(
   Vector2 origin, Vector2 padding, 
   const char* text, 
   Color buttonColor, Color textColor,
   int fontSize, std::pair<float, int> roundness // default args
) : origin(origin), roundness(roundness),text(text), fontSize(fontSize), buttonColor(buttonColor), textColor(textColor)
{ setPadding_Bounds({padding.x, padding.x}, {padding.y, padding.y}); }

Button::Button (
   Vector2 origin, 
   float paddingLeft, float paddingRight, float paddingTop, float paddingBottom, 
   const char* text, 
   Color buttonColor, Color textColor,
   int fontSize, std::pair<float, int> roundness // default args
) : origin(origin), roundness(roundness), text(text), fontSize(fontSize), buttonColor(buttonColor), textColor(textColor)
{ setPadding_Bounds({paddingLeft, paddingRight}, {paddingTop, paddingBottom}); }

bool Button::isClicked() const {
   return (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), m_bounds));
}

void Button::setFocus(bool isFocused, Color buttonColor, Color textColor) {
   this->isFocused = isFocused;
   this->buttonColor = buttonColor;
   this->textColor = textColor;
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
   if (isHovered) {
      DrawRectangleRounded(m_bounds, roundness.first, roundness.second, ColorBrightness(buttonColor, 0.169f));
      DrawRectangleRoundedLinesEx(m_bounds, roundness.first, roundness.second, 2, ColorBrightness(buttonColor, -0.1f));
   } else
      DrawRectangleRounded(m_bounds, roundness.first, roundness.second, buttonColor);

   // make padding left be only on left and right only on right etc
   Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, 1);
   Vector2 textOrigin;
   textOrigin.x = m_bounds.x + m_horizontalPadding.x + (m_bounds.width - m_horizontalPadding.x - m_horizontalPadding.y - textSize.x) / 2;
   textOrigin.y = m_bounds.y + m_verticalPadding.x + (m_bounds.height - m_verticalPadding.x -   m_verticalPadding.y - textSize.y) / 2;

  DrawTextEx(GetFontDefault(), text.c_str(), textOrigin, fontSize, 1, textColor);
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
      first.textColor == second.textColor &&
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