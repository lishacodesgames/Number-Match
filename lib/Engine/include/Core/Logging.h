#pragma once
#include <raylib.h>
#include <stdarg.h>

// LOG_NONE = 7
#define LISHA_SAYS 8

namespace Core { void LishaLogger(int msgType, const char* text, va_list args); }