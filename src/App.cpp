#include <pch/Precompiled.h>
#include "Core/Application.h"

#include "Layers/PanelLayer.h"
#include "Layers/HomeLayer.h"
#include "Storage.h"
#include "Colors.h"

Core::Application* Core::CreateApplication(const std::string& name) {
   Core::Application* app = new Application();
   app->bgColor = &Palette::off_bright_bg;
   Core::ConsoleLog(LISHA_SAYS, std::format("Loading {}...", name));

   Storage::load();

   SetConfigFlags(FLAG_WINDOW_RESIZABLE);
   std::pair<int, int> window = Storage::getSavedWindowSize();

   InitWindow(window.first, window.second, name.c_str());
   SetWindowMinSize(500, 575);
   SetTargetFPS(60);

   Storage::ui.font_retro = LoadFontEx("assets/fonts/Goldman-Regular.ttf", 20, NULL, 0);
   Storage::ui.font_semibold = LoadFontEx("assets/fonts/RedHatDisplay-SemiBold.ttf", 40, NULL, 0);
   Storage::ui.font_black = LoadFontEx("assets/fonts/RedHatDisplay-Black.ttf", 70, NULL, 0);

   if(Storage::ui.isDarkMode)
      Palette::SetDarkMode();
   else
      Palette::SetLightMode();
   
   HomeLayer* home = new HomeLayer();
   app->m_layerStack.PushLayer(home);

   PanelLayer* panel = new PanelLayer();
   app->m_layerStack.PushOverlay(panel);

   Core::ConsoleLog(LISHA_SAYS, std::format("{} Loaded!", name));
   Core::ConsoleLog(LISHA_SAYS, std::format("Save path: {}", std::filesystem::absolute(Storage::savefile).string()));
   return app;
}

void Core::DestroyApplication(Core::Application& app) {
   app.m_layerStack.Delete(); /// Must be done before CloseWindow()
   CloseWindow();
   Core::Application::s_instance = nullptr;
   Core::ConsoleLog(LISHA_SAYS, "GOODBYE!\n");
}