#include "App.h"
#include "Core/Logging.h"

int main() {
   if(_DEBUGGING) {
      SetTraceLogCallback(LishaLogger);
      SetTraceLogLevel(LOG_DEBUG);
   } else
      SetTraceLogLevel(LOG_NONE);

   App game("Number Match");
   game.Run();

   return 0;
}