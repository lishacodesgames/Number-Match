#pragma once
#include <raylib.h>
#include <stdarg.h>

// LOG_NONE = 7
#define LISHA_SAYS 8

namespace Core { void LishaLogger(int msgType, const char* text, va_list args); }

/// logs in green
#define LOG_RESIZE(output, ...) TraceLog(LOG_INFO, "\033[92mRESIZE: " output "\033[0m", ##__VA_ARGS__)

/// logs in italic underline
#define LOG_LAYER(output, ...) TraceLog(LOG_INFO, "\033[3;4mLAYER: " output "\033[0m", ##__VA_ARGS__)