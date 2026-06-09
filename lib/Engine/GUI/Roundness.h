#pragma once

namespace GUI
{
   struct Roundness {
      float roundness = 0.8f; /// 0.0f - 1.0f, 0.8f by default
      int segments = 8;       /// number of segments to use for drawing rounded corners, 8 by default
   };
}