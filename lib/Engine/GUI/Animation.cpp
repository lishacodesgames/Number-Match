#include <pch/Precompiled.h>
#include "Animation.h"

namespace GUI
{
   float LERP(float elmPos, float destination, float speedFactor, float minDistance) {
      float diff = elmPos - destination;
      
      if(std::abs(diff) < minDistance)
         return destination;
      else
         return elmPos - (0.1f * diff * speedFactor * std::min(GetFrameTime(), 0.033f));
   }
}