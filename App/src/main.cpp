#include "App.h"
#include "Core/Logging.h"

#if _DEBUGGING
   #define LOG_LEVEL LOG_WARNING
#else
   #define LOG_LEVEL LISHA_SAYS
#endif

int main() {
   SetTraceLogCallback(Core::LishaLogger);
   SetTraceLogLevel(LOG_LEVEL);

   App game("Number Match");
   game.Run();

   return 0;
}