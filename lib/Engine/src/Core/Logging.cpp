#include <pch/Precompiled.h>
#include "Core/Logging.h"

#include <raylib.h>
#include <cstdio>

void LishaLogger(int msgType, const char* text, va_list args) {
   // prefix text
   switch(msgType) {
      // raylib calls
      case LOG_TRACE: printf("[TRACE]: "); break;
      case LOG_DEBUG: printf("[DEBUG]: "); break;
      case LOG_INFO: printf("[INFO]: "); break;
      case LOG_WARNING: printf("[WARNING]: "); break;
      case LOG_ERROR: printf("[ERROR]: "); break;
      case LOG_FATAL: printf("[FATAL]: "); break;

      // info abt own app
      case LISHA_SAYS: printf("[LISHA SAYS]: "); break;
   }

   // print msg
   vprintf(text, args);
   printf("\n");
}