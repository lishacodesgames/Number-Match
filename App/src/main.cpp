#include "App.h"
#include "Core/Logging.h"

int main() {
   SetTraceLogCallback(LishaLogger);

   App game("Number Match");
   game.Run();

   return 0;
}