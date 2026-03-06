#include <Precompiled.h>
#include <raylib.h>
#include <raymath.h>
#include <Button.h>

Button::Button(Rectangle exactBounds, const char* text, Color buttonColor,Color textColor) 
   : m_bounds(exactBounds), text(text), buttonColor(buttonColor), textColor(textColor)
{
   origin = { m_bounds.x, m_bounds.y };

   float x = m_bounds.width / 2 - MeasureTextEx(GetFontDefault(), text, 20, 1).x / 2;
   float y = m_bounds.height / 2 - MeasureTextEx(GetFontDefault(), text, 20, 1).y / 2;

   m_horizontalPadding = { x, x };
   m_verticalPadding = { y,  y };
}

Button::Button(Vector2 origin, Vector2 padding, const char* text, Color buttonColor, Color textColor) 
   : text(text), buttonColor(buttonColor), textColor(textColor), origin(origin)
{
   m_horizontalPadding = { padding.x, padding.x };
   m_verticalPadding = { padding.y, padding.y };

   m_bounds.x = origin.x;
   m_bounds.y = origin.y;
   m_bounds.width = MeasureTextEx(GetFontDefault(), text, 20, 1).x + padding.x * 2;
   m_bounds.height = MeasureTextEx(GetFontDefault(), text, 20, 1).y + padding.y * 2;
}

Button::Button
   (Vector2 origin, Vector2 padding, const char* text, int fontSize, Color buttonColor, Color textColor) 
   : origin(origin), text(text), fontSize(fontSize), buttonColor(buttonColor), textColor(textColor)
{
   m_horizontalPadding = { padding.x, padding.x };
   m_verticalPadding = { padding.y, padding.y };

   m_bounds.x = origin.x;
   m_bounds.y = origin.y;
   m_bounds.width = MeasureTextEx(GetFontDefault(), text, fontSize, 1).x + padding.x * 2;
   m_bounds.height = MeasureTextEx(GetFontDefault(), text, fontSize, 1).y + padding.y * 2;
}

Button::Button
   (Vector2 origin, float paddingLeft, float paddingRight, float paddingTop, float paddingBottom, 
      const char* text, Color buttonColor, Color textColor) 
   : origin(origin), text(text), buttonColor(buttonColor), textColor(textColor)
{
   m_horizontalPadding = { paddingLeft, paddingRight };
   m_verticalPadding = { paddingTop, paddingBottom };

   m_bounds.x = origin.x;
   m_bounds.y = origin.y;
   m_bounds.width = MeasureTextEx(GetFontDefault(), text, 20, 1).x + paddingLeft + paddingRight;
   m_bounds.height = MeasureTextEx(GetFontDefault(), text, 20, 1).y + paddingTop + paddingBottom;
}

Button::Button
   (Vector2 origin, float paddingLeft, float paddingRight, float paddingTop, float paddingBottom, 
      const char* text, int fontSize, Color buttonColor, Color textColor) 
   : origin(origin), text(text), fontSize(fontSize), buttonColor(buttonColor), textColor(textColor)
{
   m_horizontalPadding = { paddingLeft, paddingRight };
   m_verticalPadding = { paddingTop, paddingBottom };

   m_bounds.x = origin.x;
   m_bounds.y = origin.y;
   m_bounds.width = MeasureTextEx(GetFontDefault(), text, fontSize, 1).x + paddingLeft + paddingRight;
   m_bounds.height = MeasureTextEx(GetFontDefault(), text, fontSize, 1).y + paddingTop + paddingBottom;
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
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isHovered)
    isActive = true;
  else
    isActive = false;
}

void Button::Draw() {
   if (isHovered) {
      DrawRectangleRounded(m_bounds, 0.8f, 10, ColorBrightness(buttonColor, 0.169f));
      DrawRectangleRoundedLinesEx(m_bounds, 0.8f, 10, 2, ColorBrightness(buttonColor, -0.1f));
   } else
      DrawRectangleRounded(m_bounds, 0.8f, 10, buttonColor);

   // make padding left be only on left and right only on right etc
   Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 1);
   Vector2 textOrigin;
   textOrigin.x = m_bounds.x + m_horizontalPadding.x + (m_bounds.width - m_horizontalPadding.x - m_horizontalPadding.y - textSize.x) / 2;
   textOrigin.y = m_bounds.y + m_verticalPadding.x + (m_bounds.height - m_verticalPadding.x -   m_verticalPadding.y - textSize.y) / 2;

  DrawTextEx(GetFontDefault(), text, textOrigin, fontSize, 1, textColor);
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