#pragma once
#include <raylib.h>

/// Customisable shadows for RECTANGLES
class Shadow {
public:
   Vector2 offset = {10, 10}; /// How far the shadow is shifted
   float softness = 2.0f;     /// How fuzzy the edges are
   Vector4 color = {0, 0, 0, 0};    /// The color and transparency of the shadow

   Shadow(Vector2 offset, float softness, Color color);
   ~Shadow();

   void Draw(Rectangle target, Vector2 offset, float softness, Vector4 color);

private:
   Shader m_shader;           /// The actual shader program loaded onto the GPU
   int m_size_memLoc;         /// Memory location ID for the 'size' variable in the shader
   int m_offset_memLoc;       /// Memory location ID for the 'offset' variable in the shader
   int m_softness_memLoc;     /// Memory location ID for the 'softness' variable in the shader
   int m_color_memLoc;        /// Memory location ID for the 'color' variable in the shader
};