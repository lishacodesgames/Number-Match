#pragma once
#include <raylib.h>
#include <stdarg.h>

#define LISHA_SAYS 8

void LishaLogger(int msgType, const char* text, va_list args);