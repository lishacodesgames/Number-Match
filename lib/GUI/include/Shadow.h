#pragma once
#include <raylib.h>

/// Customisable shadows for RECANGLES
class Shadow {
public:
   Vector2 offset; /// How far the shadow is shifted
   float blur;     /// How fuzzy the edges are
   Color color;    /// The color and transparency of the shadow

   Shadow(const char* shaderFilePath);
   ~Shadow();

   void Draw(Rectangle target, Vector2 offset, float softness, Color color);

private:
   Shader m_shader;           /// The actual shader program loaded onto the GPU
   int m_size_memLoc;         /// Memory location ID for the 'size' variable in the shader
   int m_offset_memLoc;       /// Memory location ID for the 'offset' variable in the shader
   int m_softness_memLoc;     /// Memory location ID for the 'softness' variable in the shader
   int m_color_memLoc;        /// Memory location ID for the 'color' variable in the shader
};