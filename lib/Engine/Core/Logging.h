#pragma once
#include <source_location>
#include <string>

// LOG_NONE = 7
#define LISHA_TRACE 8
#define LISHA_SAYS 9

#ifdef _DEBUGGING
   #define LOG_LEVEL LOG_INFO
#else
   #define LOG_LEVEL LOG_WARNING
#endif

namespace Core
{
   /// @todo maybe make into a macro somehow so caller doesn't have to std::format() for log everytime
   void ConsoleLog(int msgType, const std::string& log, std::source_location src = std::source_location::current());
   void LishaLogger(int msgType, const char* text, va_list args);
}

// I'm using ISO C++20 standard
// __VA_OPT__ inserts a comma only if __VA_ARGS__ exist
// The comma would still be inserted if __VA_ARGS__ was called in the macro but no args were given
// whiuch would cause syntax errors cuz it would expand to FUNC(arg2, );

// first arg has to be converted to std::string bcz string literals are const char* and therefore immutable

/// logs in green
#define LOG_RESIZE(log, ...) Core::ConsoleLog(LISHA_TRACE, std::string("\033[92mRESIZE: ") + std::format(log __VA_OPT__(,) __VA_ARGS__) + "\033[0m" );

/// logs in italic underline
#define LOG_LAYER(log, ...) Core::ConsoleLog(LISHA_TRACE, std::string("\033[3;4mLAYER: ") + std::format(log __VA_OPT__(,) __VA_ARGS__) + "\033[0m" );