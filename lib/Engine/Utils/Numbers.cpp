#include <pch/Precompiled.h>
#include "Numbers.h"

namespace Utils
{
   float avgAbsDiff(Vector2 first, Vector2 second) {
      return (std::abs(first.x - second.x) + std::abs(first.y - second.y)) / 2.0f;
   }

   std::string formatNumber(int number) {
      std::string result = std::to_string(number);
      for(std::ptrdiff_t position = static_cast<std::ptrdiff_t>(result.size()) - 3;
         position > 0; position -= 3)
      result.insert(static_cast<size_t>(position), ",");

      return result;
   }
}