#include <pch/Precompiled.h>
#include "Button.h"

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

   void Button::Draw() const {
      // draw button
      if(!isHovered)
         DrawRectangleRounded(m_bounds, m_roundness.roundness, m_roundness.segments, bgColor);
      else {
         if(bgColor == WHITE)  // darker for contrast cuz nothings lighter than white
            DrawRectangleRounded(
               m_bounds, m_roundness.roundness, m_roundness.segments, ColorBrightness(bgColor, -0.04232f)
            );
         else  // lighter cuz it gives a satisfying pseudo-growth to button
            DrawRectangleRounded(
               m_bounds, m_roundness.roundness, m_roundness.segments, ColorBrightness(bgColor, 0.069f)
            );

         DrawRectangleRoundedLinesEx(
            m_bounds, m_roundness.roundness, m_roundness.segments, 2, ColorBrightness(bgColor, -0.12f)
         );
      }

      // helper variables for calculation
      Vector2 iconOrigin = getIconOrigin();
      Vector2 iconSize = getIconSize();

      /// @bug fix button icon rendering
      if(m_iconTexture)
         DrawTextureEx(*m_iconTexture, iconOrigin, 0.0f, iconSize.y / m_iconTexture->height, contentColor);

      // center text
      float remTextSpaceY = m_bounds.height - m_verticalPadding.x - m_verticalPadding.y - getTextSize().y;
      float textOriginY = m_bounds.y + m_verticalPadding.x + remTextSpaceY / 2;

      DrawTextEx(m_font, m_text.c_str(), getTextOrigin(), m_fontSize, 1, contentColor);
   }

#pragma endregion

#pragma region Constructors
   Button::~Button() {
      if(m_iconTexture)
         UnloadTexture(*m_iconTexture);
   }

   Button::Button(
      Rectangle exactBounds,
      const char* text,
      Color bgColor, Color contentColor,
      int fontSize, Roundness roundness,  // default args
      Font font
   ) : m_bounds(exactBounds), m_roundness(roundness), m_text(text), m_fontSize(fontSize), bgColor(bgColor), contentColor(contentColor), m_font(font)
   {
      Vector2 textSize = getTextSize();
      float x = (m_bounds.width - textSize.x) / 2;
      float y = (m_bounds.height - textSize.y) / 2;

      m_horizontalPadding = { x, x };
      m_verticalPadding = { y, y };
   }

   Button::Button(
      Vector2 origin, Vector2 padding, 
      const char* text, 
      Color bgColor, Color contentColor,
      int fontSize, Roundness roundness, // default args
      Font font
   ) : m_roundness(roundness),m_text(text), m_fontSize(fontSize), bgColor(bgColor), contentColor(contentColor), m_font(font)
   {
      setOrigin(origin);
      setPadding({ padding.x, padding.x }, { padding.y, padding.y });
   }

   Button::Button (
      Vector2 origin, 
      Vector2 horizPadding, Vector2 vertPadding, 
      const char* text, 
      Color bgColor, Color contentColor,
      int fontSize, Roundness roundness, // default args
      Font font
   ) : m_roundness(roundness), m_text(text), m_fontSize(fontSize), bgColor(bgColor), contentColor(contentColor), m_font(font)
   {
      setOrigin(origin);
      setPadding(horizPadding, vertPadding);
   }
#pragma endregion

#pragma region Setters
   void Button::setIcon(const char* filepath, Vector2 dimensions) {  // dimensions = {0, 0} as default args
      Image img = LoadImage(filepath);
      if(!dimensions.x || !dimensions.y) {  // any are 0
         dimensions.x = img.width;
         dimensions.y = img.height;
      } else {
         ImageResize(&img, dimensions.x, dimensions.y);
      }


      if(m_iconTexture)
         UnloadTexture(*m_iconTexture);
      m_iconTexture = LoadTextureFromImage(img);
      UnloadImage(img);

      recalculateLayout();
   }

   void Button::setFontSize(float fontSize) {
      m_fontSize = fontSize;
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
      m_fontSize *= heightScale;
   }

   void Button::setPadding(Vector2 horizPadding, Vector2 vertPadding) {
      m_horizontalPadding = horizPadding;
      m_verticalPadding = vertPadding;

      recalculateLayout();
   }

   void Button::setFocus(bool isFocused, Color bgColor, Color contentColor) {
      this->isFocused = isFocused;
      this->bgColor = bgColor;
      this->contentColor = contentColor;
   }

   void Button::recalculateLayout() {
      Vector2 iconSize = getIconSize();
      Vector2 textSize = getTextSize();
      float contentWidth = textSize.x + (m_iconTexture ? iconSize.x * (m_text.empty() ? 1 : ICON_PAD_MULTIPLIER) : 0);
      float contentHeight = std::max(textSize.y, iconSize.y);

      m_bounds.width = contentWidth + m_horizontalPadding.x + m_horizontalPadding.y;
      m_bounds.height = contentHeight + m_verticalPadding.x + m_verticalPadding.y;
   }
#pragma endregion

#pragma region Getters
   Vector2 Button::getIconOrigin() const { 
      if(!m_iconTexture)
         return { -1, -1 };

      Vector2 iconSize = getIconSize();
      float contentWidth = getTextSize().x + iconSize.x * (m_text.empty() ? 1 : ICON_PAD_MULTIPLIER);

      // center X
      float remSpaceX = m_bounds.width - m_horizontalPadding.x - m_horizontalPadding.y - contentWidth;
      float originX = m_bounds.x + m_horizontalPadding.x + remSpaceX / 2;  // centered horizontally

      // center Y
      float remSpaceY = m_bounds.height - m_verticalPadding.x - m_verticalPadding.y - iconSize.y;
      float originY = m_bounds.y + m_verticalPadding.x + remSpaceY / 2;

      return { originX, originY };
   }

   Vector2 Button::getTextOrigin() const {
      if(m_text.empty())
         return { -1, -1 };  // invalid origin since no text to draw

      Vector2 textSize = getTextSize();
      float iconWidth = getIconSize().x;
      float gap = m_iconTexture ? iconWidth * (ICON_PAD_MULTIPLIER - 1.0f) : 0.0f; // between icon and text
      float contentWidth = textSize.x + iconWidth * ICON_PAD_MULTIPLIER;

      // center X
      float remSpaceX = m_bounds.width - m_horizontalPadding.x - m_horizontalPadding.y - contentWidth;
      float originX = m_bounds.x + m_horizontalPadding.x + remSpaceX / 2 + iconWidth * ICON_PAD_MULTIPLIER;  // centered horizontally

      // center Y
      float remSpaceY = m_bounds.height - m_verticalPadding.x - m_verticalPadding.y - textSize.y;
      float originY = m_bounds.y + m_verticalPadding.x + remSpaceY / 2;

      return { originX, originY };
   }

   Vector2 Button::getIconSize() const {
      if (!m_iconTexture)
         return { 0, 0 };

      float scale = m_fontSize / m_iconTexture->height;
      return { m_iconTexture->width * scale, m_fontSize };
   }

   Vector2 Button::getTextSize() const {
      return MeasureTextEx(m_font, m_text.c_str(), m_fontSize, 1.0f);
   }

#pragma endregion
}  // namespace GUI

#pragma region Operators

bool operator==(const Color& first, const Color& second) {
   return
      first.r == second.r &&
      first.g == second.g &&
      first.b == second.b &&
      first.a == second.a;
}

bool operator==(const Rectangle& first, const Rectangle& second) {
   return
      first.x == second.x &&
      first.y == second.y &&
      first.width == second.width &&
      first.height == second.height;
}

bool operator==(const Texture2D& first, const Texture2D& second) {
   return
      first.id == second.id &&
      first.width == second.width &&
      first.height == second.height &&
      first.mipmaps == second.mipmaps &&
      first.format == second.format;
}

bool operator==(const Font& first, const Font& second) {
   return
      first.baseSize == second.baseSize &&
      first.glyphCount == second.glyphCount &&
      first.glyphPadding == second.glyphPadding &&
      first.texture == second.texture &&
      first.recs == second.recs &&
      first.glyphs == second.glyphs;
}

#pragma endregion
