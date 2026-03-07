#include <Precompiled.h>
#include "Shadow.h"

#include <raylib.h>

Shadow::Shadow(const char* shaderFilePath) {
   // Load the shader from the file path provided
   m_shader = LoadShader(0, shaderFilePath); // 0 = raylib's default vertex shader
   
   // Get address of uniforms from shadow.fs
   m_size_memLoc = GetShaderLocation(m_shader, "size");
   m_offset_memLoc = GetShaderLocation(m_shader, "offset");
   m_softness_memLoc = GetShaderLocation(m_shader, "softness");
   m_color_memLoc = GetShaderLocation(m_shader, "color");
}

Shadow::~Shadow() { UnloadShader(m_shader); }

void Shadow::Draw(Rectangle target, Vector2 offset, float softness, Color color) {
   // Prepare data for the shader (Convert to floats/Vector2)
   Vector2 size = { target.width, target.height };
   Vector4 floatColor = ColorNormalize(color);

   // -------------
   // SHADER WORK
   // -------------

   BeginShaderMode(m_shader);

   // send C++ data to GPU
   SetShaderValue(m_shader, m_size_memLoc, &size, SHADER_UNIFORM_VEC2);
   SetShaderValue(m_shader, m_offset_memLoc, &offset, SHADER_UNIFORM_VEC2);
   SetShaderValue(m_shader, m_softness_memLoc, &softness, SHADER_UNIFORM_FLOAT);
   SetShaderValue(m_shader, m_color_memLoc, &color, SHADER_UNIFORM_VEC4);

   // draw mesh
   float padding = softness * 2;
   DrawRectangle(
      static_cast<int>(target.x - padding),
      static_cast<int>(target.y - padding),
      static_cast<int>(target.width + padding*2),
      static_cast<int>(target.height + padding*2),
      WHITE // shader calculates its own color so this remains white
   );

   EndShaderMode();
}