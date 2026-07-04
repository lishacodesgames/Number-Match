#include "Core/Application.h"
#include "Core/Logging.h"
#include "Colors.h"

int main() {
   SetTraceLogCallback(Core::LishaLogger);
   SetTraceLogLevel(LOG_LEVEL);

   Core::Application* game = Core::CreateApplication("Number Match");
   game->Run();
}