#include "App.h"
#include "Core/Logging.h"

#if _DEBUGGING
   #define LOG_LEVEL LOG_DEBUG
#else
   #define LOG_LEVEL 10 // out of range, no log will print.
#endif

int main() {
   SetTraceLogCallback(LishaLogger);
   SetTraceLogLevel(LOG_LEVEL);

   App game("Number Match");
   game.Run();

   return 0;
}