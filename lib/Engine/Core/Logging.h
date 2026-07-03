#pragma once
#include <source_location>
#include <string>

// LOG_NONE = 7
#define LISHA_TRACE 8
#define LISHA_SAYS 9

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

#ifdef _DEBUGGING
   #define LOG_LEVEL LOG_INFO
#else
   #define LOG_LEVEL LOG_WARNING
#endif


namespace Core
{
   void ConsoleLog(int msgType, const std::string& log, std::source_location src = std::source_location::current());
   void LishaLogger(int msgType, const char* text, va_list args);
}