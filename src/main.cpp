#include "App.h"
#include "Core/Logging.h"

int main() {
   SetTraceLogCallback(Core::LishaLogger);
   SetTraceLogLevel(LOG_LEVEL);

   App game("Number Match");
   game.Run();
}