#pragma once
#include <raylib.h>
#include <string>

namespace Utils
{
   float avgAbsDiff(Vector2 first, Vector2 second);
   std::string formatNumber(int number); /// Formats a number with commas as thousands separators
}