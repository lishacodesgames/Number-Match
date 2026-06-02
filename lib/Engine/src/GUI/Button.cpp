#include <pch/Precompiled.h>
#include "GUI/Button.h"

#include <algorithm>
#include <raymath.h>
#include <raylib.h>
#include <utility>

namespace GUI 
{
   static constexpr float ICON_PAD_MULTIPLIER = 1.5f;

#pragma region Methods
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

      // helper variables for calculation
      Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 1);
      Vector2 iconOrigin = getIconOrigin();

      if(IsTextureValid(icon))
         DrawTexture(icon, iconOrigin.x, iconOrigin.y, contentColor);

      //center text
      float remTextSpaceY = m_bounds.height - m_verticalPadding.x - m_verticalPadding.y - textSize.y;
      float textOriginY = m_bounds.y + m_verticalPadding.x + remTextSpaceY/2;

      DrawTextEx(font, text.c_str(), {iconOrigin.x + icon.width * ICON_PAD_MULTIPLIER, textOriginY}, fontSize, 1, contentColor);
   }

#pragma endregion

#pragma region Constructors
   Button::~Button() {
      if(IsTextureValid(icon))
         UnloadTexture(icon);
      if(IsImageValid(m_icon))
         UnloadImage(m_icon);
   }

   Button::Button(
      Rectangle exactBounds, 
      const char* text, 
      Color buttonColor, Color contentColor,
      int fontSize, std::pair<float, int> roundness, // default args
      Font font
   ) 
      : m_bounds(exactBounds), roundness(roundness), text(text), fontSize(fontSize), buttonColor(buttonColor), contentColor(contentColor), font(font)
   {
      Vector2 textSize = MeasureTextEx(font, text, fontSize, 1);
      float x = (m_bounds.width - textSize.x)/2;
      float y = (m_bounds.height - textSize.y)/2;
      
      m_horizontalPadding = { x, x };
      m_verticalPadding = { y,  y };
   }

   Button::Button(
      Vector2 origin, Vector2 padding, 
      const char* text, 
      Color buttonColor, Color contentColor,
      int fontSize, std::pair<float, int> roundness, // default args
      Font font
   ) : roundness(roundness),text(text), fontSize(fontSize), buttonColor(buttonColor), contentColor(contentColor), font(font)
   {
      setOrigin(origin);
      setPadding({padding.x, padding.x}, {padding.y, padding.y});
   }

   Button::Button (
      Vector2 origin, 
      float paddingLeft, float paddingRight, float paddingTop, float paddingBottom, 
      const char* text, 
      Color buttonColor, Color contentColor,
      int fontSize, std::pair<float, int> roundness, // default args
      Font font
   ) : roundness(roundness), text(text), fontSize(fontSize), buttonColor(buttonColor), contentColor(contentColor), font(font)
   {
      setOrigin(origin);
      setPadding({paddingLeft, paddingRight}, {paddingTop, paddingBottom}); 
   }
#pragma endregion

#pragma region Setters
   void Button::setIcon(const char* filepath, Vector2 dimensions) { // dimensions = {0, 0} as default args
      m_icon = LoadImage(filepath);
      if(!dimensions.x || !dimensions.y) {// any are 0
         dimensions.x = m_icon.width;
         dimensions.y = m_icon.height;
      }
      ImageResize(&m_icon, dimensions.x, dimensions.y);
      this->icon = LoadTextureFromImage(m_icon);

      setPadding(m_horizontalPadding, m_verticalPadding);
   }

   void Button::setOrigin(int x, int y) {
      m_bounds.x = static_cast<float>(x);
      m_bounds.y = static_cast<float>(y);
   }

   void Button::setOrigin(Vector2 origin) {
      m_bounds.x = origin.x;
      m_bounds.y = origin.y;
   }

   void Button::setSize(int fontSize, Vector2 padding) {
      /// @bug icon resizing doesn't preserve icon's aspect ratio
      bool iconExists = IsTextureValid(icon);
      this->fontSize = fontSize;
      Vector2 iconRatio = iconExists ? Vector2{(float)icon.width / m_bounds.width, (float)icon.height / m_bounds.height} : Vector2{0, 0};
      if(padding.x >= 0 && padding.y >= 0) // if padding is provided as an argument, we use it, otherwise we keep the same padding
         setPadding({padding.x, padding.y}, {padding.x, padding.y});
      
      if(iconExists) {
         ImageResize(&m_icon, m_bounds.width * iconRatio.x, m_bounds.height * iconRatio.y);
         UnloadTexture(icon);
         icon = LoadTextureFromImage(m_icon);
      }
   }

   void Button::setSize(Vector2 size) {
      m_bounds.width = size.x;
      m_bounds.height = size.y;
   }

   void Button::setBounds(Rectangle bounds) { m_bounds = bounds; }

   void Button::setPadding(Vector2 horizPadding, Vector2 vertPadding) {
      this->m_horizontalPadding = horizPadding;
      this->m_verticalPadding = vertPadding;

      Vector2 size = MeasureTextEx(font, text.c_str(), fontSize, 1);
      if(IsTextureValid(icon)) {
         size.x += icon.width * (text.empty()? 1:ICON_PAD_MULTIPLIER);
         size.y = std::max(size.y, static_cast<float>(icon.height));
      }

      m_bounds.width = size.x + horizPadding.x + horizPadding.y;
      m_bounds.height = size.y + vertPadding.x + vertPadding.y;
   }

   void Button::setFocus(bool isFocused, Color buttonColor, Color contentColor) {
      this->isFocused = isFocused;
      this->buttonColor = buttonColor;
      this->contentColor = contentColor;
   }
#pragma endregion

#pragma region Getters
   Vector2 Button::getIconOrigin() const {
      Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 1);
      float contentWidth = textSize.x + icon.width * ICON_PAD_MULTIPLIER;

      // center X
      float remSpaceX = m_bounds.width - m_horizontalPadding.x - m_horizontalPadding.y - contentWidth;
      float originX = m_bounds.x  + m_horizontalPadding.x + remSpaceX/2; // centered horizontally

      // center Y
      float remSpaceY = m_bounds.height - m_verticalPadding.x - m_verticalPadding.y - icon.height;
      float originY = m_bounds.y + m_verticalPadding.x + remSpaceY/2;

      return {originX, originY};
   }
   Vector2 Button::getOrigin() const { return { m_bounds.x, m_bounds.y }; }
   Vector2 Button::getSize() const { return {m_bounds.width, m_bounds.height}; }
   Rectangle Button::getBounds() const { return m_bounds; }
#pragma endregion
} // namespace GUI

#pragma region Operators
bool GUI::operator==(const GUI::Button& first, const GUI::Button& second) {
   return (
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
#pragma endregion