#include <Precompiled.h>
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

bool Button::isHovering() {
  Vector2 mousePos = GetMousePosition();

  return CheckCollisionPointRec(mousePos, m_bounds);
}

bool Button::isClicked() {
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, m_bounds)) return true;
  }
  return false;
}

void Button::Draw()  {
   if(isHovering()) {
      DrawRectangleRounded(m_bounds, 0.8f, 10, ColorBrightness(buttonColor, 0.169f));
      DrawRectangleRoundedLinesEx(m_bounds, 0.8f, 10, 2, ColorBrightness(buttonColor, -0.1f));
   }
   else
      DrawRectangleRounded(m_bounds, 0.8f, 10, buttonColor);


   // make padding left be only on left and right only on right etc
   Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 1);
   Vector2 textPos = { 
      m_bounds.x + m_horizontalPadding.x + 
         (m_bounds.width - m_horizontalPadding.x - m_horizontalPadding.y - textSize.x) / 2, 
      m_bounds.y + m_verticalPadding.x + (m_bounds.height - m_verticalPadding.x - m_verticalPadding.y - textSize.y) / 2 
   };

   DrawTextEx(GetFontDefault(), text, textPos, fontSize, 1, textColor);
}