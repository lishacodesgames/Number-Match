#pragma once

// LOG_NONE = 7
#define LISHA_SAYS 8

// I'm using ISO C++20 standard
// __VA_OPT__ inserts a comma only if __VA_ARGS__ exist
// The comma would still be inserted if __VA_ARGS__ was called in the macro but no args were given
// whiuch would cause syntax errors cuz it would expand to FUNC(arg2, );

/// logs in green
#define LOG_RESIZE(output, ...) \
   TraceLog(LOG_INFO, "\033[92mRESIZE: " output "\033[0m" __VA_OPT__(,) __VA_ARGS__)

/// logs in italic underline
#define LOG_LAYER(output, ...) \
   TraceLog(LOG_INFO, "\033[3;4mLAYER: " output "\033[0m" __VA_OPT__(,) __VA_ARGS__)

namespace Core
{
   void LishaLogger(int msgType, const char* text, va_list args);
}