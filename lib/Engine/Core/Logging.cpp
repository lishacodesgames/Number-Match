#include <pch/Precompiled.h>
#include "Logging.h"

namespace Core
{
   void LishaLogger(int msgType, const char* text, va_list args) {
      // prefix text
      switch(msgType) {
         // raylib calls

         case LOG_TRACE: printf("[TRACE]: "); break;
         case LOG_DEBUG: printf("[DEBUG]: "); break;
         case LOG_INFO: printf("[INFO]: "); break;

         // bold, underline, slow blinking, yellow text
         case LOG_WARNING: printf("\033[1;4;5;33m[WARNING]:\033[0m "); break;

         // bold, underline, rapid blinking, red text, yellow bg 
         case LOG_ERROR: printf("\033[1;4;6;91;103m[ERROR]:\033[0m "); break;

         // black text, white bg
         case LOG_FATAL: printf("\033[30;47m[FATAL]:\033[0m "); break;

         // info abt own app : bold, bright/light blue text
         case LISHA_SAYS: printf("\033[1;94m[LISHA SAYS]:\033[0m "); break;
      }

      // print msg
      vprintf(text, args);
      printf("\n");
   }
}