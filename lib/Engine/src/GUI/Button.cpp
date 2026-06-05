#include <pch/Precompiled.h>
#include "GUI/Button.h"

namespace GUI 
{
   static constexpr float ICON_PAD_MULTIPLIER = 1.5f;

#pragma region Methods
   void Button::Update() {
      // update hover flag
      if(CheckCollisionPointRec(GetMousePosition(), m_bounds))
         isHovered = true;
      else
         isHovered = false;

      // update active flag
      if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && isHovered)
         isActive = true;
      else
         isActive = false;
   }

   void Button::Draw() {
      // draw button
      if(!isHovered)
         DrawRectangleRounded(m_bounds, m_roundness.roundness, m_roundness.segments, m_buttonColor);
      else {
         if(m_buttonColor == WHITE)  // darker for contrast cuz nothings lighter than white
            DrawRectangleRounded(
               m_bounds, m_roundness.roundness, m_roundness.segments, ColorBrightness(m_buttonColor, -0.04232f)
            );
         else  // lighter cuz it gives a satisfying pseudo-growth to button
            DrawRectangleRounded(
               m_bounds, m_roundness.roundness, m_roundness.segments, ColorBrightness(m_buttonColor, 0.069f)
            );

         DrawRectangleRoundedLinesEx(
            m_bounds, m_roundness.roundness, m_roundness.segments, 2, ColorBrightness(m_buttonColor, -0.12f)
         );
      }

      // helper variables for calculation
      Vector2 textSize = MeasureTextEx(m_font, m_text.c_str(), m_fontSize, 1);
      Vector2 iconOrigin = getIconOrigin();

      if(IsTextureValid(m_iconTexture))
         DrawTexture(m_iconTexture, iconOrigin.x, iconOrigin.y, m_contentColor);

      // center text
      float remTextSpaceY = m_bounds.height - m_verticalPadding.x - m_verticalPadding.y - textSize.y;
      float textOriginY = m_bounds.y + m_verticalPadding.x + remTextSpaceY / 2;

      DrawTextEx(m_font, m_text.c_str(), { iconOrigin.x + m_iconTexture.width * ICON_PAD_MULTIPLIER, textOriginY }, m_fontSize, 1, m_contentColor);
   }

#pragma endregion

#pragma region Constructors
   Button::~Button() {
      if(IsTextureValid(m_iconTexture))
         UnloadTexture(m_iconTexture);
      if(IsImageValid(m_iconImage))
         UnloadImage(m_iconImage);
   }

   Button::Button(
      Rectangle exactBounds,
      const char* text,
      Color buttonColor, Color contentColor,
      int fontSize, Roundness roundness,  // default args
      Font font
   ) : m_bounds(exactBounds), m_roundness(roundness), m_text(text), m_fontSize(fontSize), m_buttonColor(buttonColor), m_contentColor(contentColor), m_font(font)
   {
      Vector2 textSize = MeasureTextEx(m_font, text, m_fontSize, 1);
      float x = (m_bounds.width - textSize.x) / 2;
      float y = (m_bounds.height - textSize.y) / 2;

      m_horizontalPadding = { x, x };
      m_verticalPadding = { y, y };
   }

   Button::Button(
      Vector2 origin, Vector2 padding, 
      const char* text, 
      Color buttonColor, Color contentColor,
      int fontSize, Roundness roundness, // default args
      Font font
   ) : m_roundness(roundness),m_text(text), m_fontSize(fontSize), m_buttonColor(buttonColor), m_contentColor(contentColor), m_font(font)
   {
      setOrigin(origin);
      setPadding({ padding.x, padding.x }, { padding.y, padding.y });
   }

   Button::Button (
      Vector2 origin, 
      Vector2 horizPadding, Vector2 vertPadding, 
      const char* text, 
      Color buttonColor, Color contentColor,
      int fontSize, Roundness roundness, // default args
      Font font
   ) : m_roundness(roundness), m_text(text), m_fontSize(fontSize), m_buttonColor(buttonColor), m_contentColor(contentColor), m_font(font)
   {
      setOrigin(origin);
      setPadding(horizPadding, vertPadding);
   }
#pragma endregion

#pragma region Setters
   void Button::setIcon(const char* filepath, Vector2 dimensions) {  // dimensions = {0, 0} as default args
      m_iconImage = LoadImage(filepath);
      if(!dimensions.x || !dimensions.y) {  // any are 0
         dimensions.x = m_iconImage.width;
         dimensions.y = m_iconImage.height;
      }
      Image resized = ImageCopy(m_iconImage);;
      ImageResize(&resized, dimensions.x, dimensions.y);
      m_iconTexture = LoadTextureFromImage(resized);

      recalculateLayout();
   }

   void Button::setFontSize(int fontSize) {
      m_fontSize = fontSize;
      if(IsTextureValid(m_iconTexture)) {
         float aspect = (float)m_iconImage.width / m_iconImage.height;
         Image newIcon = ImageCopy(m_iconImage);
         float newHeight = m_fontSize;
         ImageResize(&newIcon, newHeight * aspect, newHeight);

         UnloadTexture(m_iconTexture);
         m_iconTexture = LoadTextureFromImage(newIcon);
         UnloadImage(newIcon);
      }
      recalculateLayout();
   }

   void Button::setBounds(Vector2 bounds, bool resizeContent) {
      Vector2 oldSize = { m_bounds.width, m_bounds.height };
      m_bounds.width = bounds.x;
      m_bounds.height = bounds.y;
      if(!resizeContent)
         return;

      float widthScale = m_bounds.width / oldSize.x;
      float heightScale = m_bounds.height / oldSize.y;

      // resize relatively
      m_horizontalPadding *= widthScale;
      m_verticalPadding *= heightScale;

      m_fontSize *= m_text.empty() ? 1 : heightScale;
      if(IsTextureValid(m_iconTexture)) {
         float aspect = (float)m_iconImage.width / m_iconImage.height;
         Image newIcon = ImageCopy(m_iconImage);
         float newHeight = m_iconTexture.height * heightScale;
         ImageResize(&newIcon, newHeight * aspect, newHeight);

         UnloadTexture(m_iconTexture);
         m_iconTexture = LoadTextureFromImage(newIcon);
         UnloadImage(newIcon);
      }
   }

   void Button::setPadding(Vector2 horizPadding, Vector2 vertPadding) {
      m_horizontalPadding = horizPadding;
      m_verticalPadding = vertPadding;

      recalculateLayout();
   }

   void Button::setFocus(bool isFocused, Color buttonColor, Color contentColor) {
      this->isFocused = isFocused;
      m_buttonColor = buttonColor;
      m_contentColor = contentColor;
   }

   void Button::recalculateLayout() {
      Vector2 textSize = MeasureTextEx(m_font, m_text.c_str(), m_fontSize, 1);
      bool iconExists = IsTextureValid(m_iconTexture);
      float contentWidth = textSize.x + (iconExists ? m_iconTexture.width * (m_text.empty() ? 1 : ICON_PAD_MULTIPLIER) : 0);
      float contentHeight = std::max(textSize.y, iconExists ? (float)m_iconTexture.height : 0.0f);

      m_bounds.width = contentWidth + m_horizontalPadding.x + m_horizontalPadding.y;
      m_bounds.height = contentHeight + m_verticalPadding.x + m_verticalPadding.y;
   }
#pragma endregion

#pragma region Getters
   Vector2 Button::getIconOrigin() const {
      Vector2 textSize = MeasureTextEx(m_font, m_text.c_str(), m_fontSize, 1);
      float contentWidth = textSize.x + m_iconTexture.width * (m_text.empty() ? 1 : ICON_PAD_MULTIPLIER);

      // center X
      float remSpaceX = m_bounds.width - m_horizontalPadding.x - m_horizontalPadding.y - contentWidth;
      float originX = m_bounds.x + m_horizontalPadding.x + remSpaceX / 2;  // centered horizontally

      // center Y
      float remSpaceY = m_bounds.height - m_verticalPadding.x - m_verticalPadding.y - m_iconTexture.height;
      float originY = m_bounds.y + m_verticalPadding.x + remSpaceY / 2;

      return { originX, originY };
   }

   Vector2 Button::getTextOrigin() const {
      if(m_text.empty())
         return { -1, -1 };  // invalid origin since no text to draw
      
      Vector2 textSize = MeasureTextEx(m_font, m_text.c_str(), m_fontSize, 1);
      float contentWidth = textSize.x + m_iconTexture.width * ICON_PAD_MULTIPLIER;

      // center X
      float remSpaceX = m_bounds.width - m_horizontalPadding.x - m_horizontalPadding.y - contentWidth;
      float originX = m_bounds.x + m_horizontalPadding.x + remSpaceX / 2 + m_iconTexture.width * ICON_PAD_MULTIPLIER;  // centered horizontally

      // center Y
      float remSpaceY = m_bounds.height - m_verticalPadding.x - m_verticalPadding.y - textSize.y;
      float originY = m_bounds.y + m_verticalPadding.x + remSpaceY / 2;

      return { originX, originY };
   }
#pragma endregion
}  // namespace GUI

#pragma region Operators
bool GUI::operator==(const GUI::Button& first, const GUI::Button& second) {
   return (
      first.m_text == second.m_text &&
      first.m_fontSize == second.m_fontSize &&
      first.m_buttonColor == second.m_buttonColor &&
      first.m_contentColor == second.m_contentColor &&
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