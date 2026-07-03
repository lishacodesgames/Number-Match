#include <pch/Precompiled.h>
#include "Logging.h"

static bool shouldTrace(int msgType) {
   return
      msgType == LOG_TRACE ||
      msgType == LOG_DEBUG ||
      msgType == LOG_WARNING ||
      msgType == LOG_ERROR ||
      msgType == LOG_FATAL;
}

namespace Core
{
   void ConsoleLog(int msgType, const std::string& log, std::source_location src) {
      if(shouldTrace(msgType))
         printf(std::format("file: {}, line: {}\n", src.file_name(), src.line()).c_str());

      TraceLog(msgType, "%s", log.c_str());
   }

   void LishaLogger(int msgType, const char* text, va_list args) {
      // prefix text
      switch(msgType) {
         case LOG_TRACE: 
            printf("[TRACE]: ");
            break;

         case LOG_DEBUG:
            printf("[DEBUG]: ");
            break;

         // only printed in Debug (not in Symbols or Release)
         case LOG_INFO:
            #ifdef _LOG_INFO
               printf("[INFO]: ");
               break;
            #else
               return;
            #endif

         // bold, underline, slow blinking, yellow text
         case LOG_WARNING:
            printf("\033[1;4;5;33m[WARNING]:\033[0m ");
            break;

         // bold, underline, rapid blinking, red text, yellow bg 
         case LOG_ERROR:
            printf("\033[1;4;6;91;103m[ERROR]:\033[0m ");
            break;

         // black text, white bg
         case LOG_FATAL:
            printf("\033[30;47m[FATAL]:\033[0m ");
            break;

         // info abt own app but will be stripped in Release 
         // bold, bright/light pink text
         case LISHA_TRACE:
            #ifdef _DEBUGGING
               printf("\033[1;38:5:212m[LISHA TRACE]:\033[0m ");
               break;
            #else
               return;
            #endif

         // info abt own app that I want logged in Release builds
         // bold, bright/light blue text
         case LISHA_SAYS:
            printf("\033[1;94m[LISHA SAYS]:\033[0m ");
            break;
      }

      // print msg
      vprintf(text, args);
      printf("\n");
   }
}