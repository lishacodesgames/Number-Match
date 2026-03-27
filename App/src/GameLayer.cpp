#include <pch/Precompiled.h>
#include "GameLayer.h"

#include "Core/Logging.h"
#include "OptionsLayer.h"
#include "Core/Layer.h"
#include "PanelLayer.h"
#include "HomeLayer.h"
#include "App.h"

static constexpr Vector2 helperPadding = {8, 8};

GameLayer::GameLayer() : Core::Layer("Game Layer"),
      m_grid(9, {0, 0, 0, 0, 0, 0, 0, 0, 0}), // 9 rows of all blank cells
      m_gridBox({(float)(GetScreenWidth())/2 - 202.5f, 130, 405, 405}),
      m_gobackButton({15, 15}, {0, 0}, "", BLANK, Color{42, 187, 235, 255}, 20, {0, 0}),
      m_settingsButton(
         {static_cast<float>(GetScreenWidth()) - 45, 15}, {0, 0}, "", BLANK, Color{42, 187, 235, 255}, 20, {0, 0}
      ),
      m_plusButton(
         {static_cast<float>(GetScreenWidth()) / 2 - 49, static_cast<float>(GetScreenHeight()) - 60}, // origin
         helperPadding, "", LIGHTERGRAY, BLUE, 25, {1.0f, 8}
      ),
      m_hintButton(
         {static_cast<float>(GetScreenWidth()) / 2 + 14, static_cast<float>(GetScreenHeight()) - 60}, // origin
         helperPadding, "", LIGHTERGRAY, BLUE, 25, {1.0f, 8}
      )
{      
   m_gobackButton.setIcon("assets/icons/game/goback_18x24.png");
   m_settingsButton.setIcon("assets/icons/game/settings_30x30.png");
   m_plusButton.setIcon("assets/icons/game/plus_35x35.png");
   m_hintButton.setIcon("assets/icons/game/hint_35x35.png");

   initGrid();
}

void GameLayer::OnAttach() {
   SetMouseCursor(MOUSE_CURSOR_DEFAULT);
   Core::Layer::OnAttach();   
}

void GameLayer::OnEvent(Core::Event& e) {
   if(isSuspended && !eventSuspended)
      return;

   if(e.GetEventType() == Core::EventType::KeyPressed) {
      char key = static_cast<Core::KeyPressedEvent&>(e).key;
      if(key == 'q' || key == 'Q') {
         OnSuspend();
         App::QueueLayerPush(new HomeLayer());
         App::QueueLayerPush(new PanelLayer());
         e.Handled = true;
      }
   } else if(e.GetEventType() == Core::EventType::MouseClicked) {
      GUI::Button* activeButton = findHoveredButton();

      if(activeButton == &m_gobackButton) {
         OnSuspend();
         App::QueueLayerPush(new HomeLayer());
         App::QueueLayerPush(new PanelLayer());
         e.Handled = true;
      } else if(activeButton == &m_settingsButton) {
         OnSuspend(true); // suspend but render
         App::QueueLayerPush(new OptionsLayer());
         e.Handled = true;
      } else if(activeButton == &m_plusButton) {
         TraceLog(LISHA_SAYS, "PLUS"); // temp
      } else if(activeButton == &m_hintButton) {
         TraceLog(LISHA_SAYS, "HINT"); // temp
      }
   }
}

void GameLayer::OnUpdate() {
   if(isSuspended && !updateSuspended)
      return;
   
   m_gobackButton.Update();
   m_settingsButton.Update();
   m_plusButton.Update();
   m_hintButton.Update();

   if(findHoveredButton() || CheckCollisionPointRec(GetMousePosition(), m_gridBox))
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
   else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void GameLayer::OnRender() {
   if(isSuspended && !renderSuspended)
      return;

   m_gobackButton.Draw();
   m_settingsButton.Draw();
   m_plusButton.Draw();
   m_hintButton.Draw();

   // Grid
   constexpr float cellSize = 45; // 405 / 9 = 45
   std::string num;
   Vector2 numSize;
   Rectangle square;
   for(unsigned int row = 0; row < m_grid.size(); row++) {
      for(unsigned int col = 0; col < m_grid.at(row).size(); col++) {
         num = std::to_string(m_grid[row][col]);
         numSize = MeasureTextEx(App::font_semibold, num.c_str(), 40, 1);
         square = {m_gridBox.x + col * cellSize, m_gridBox.y + row * cellSize, cellSize, cellSize};

         DrawRectangleLinesEx(square, 1, ColorAlpha(LIGHTGRAY, 0.65f));
         DrawTextEx(
            App::font_semibold, num.c_str(),
            {square.x + square.width/2 - numSize.x/2, square.y + square.height/2 - numSize.y/2},
            40, 1, BLACK
         );
      }
   }

   // Box
   DrawRectangleLinesEx(m_gridBox, 3, ColorAlpha(DARKGRAY, 0.8f));
}

void GameLayer::initGrid() {
   srand(time(0));
   for(auto& row : m_grid) {
      for(int& cell : row) {
         cell = 1 + rand() % 8;
      }
   }
}

#pragma region Helpers

GUI::Button* GameLayer::findHoveredButton() {
   if (m_gobackButton.isHovered)
      return &m_gobackButton;
   else if (m_settingsButton.isHovered)
      return &m_settingsButton;
   else if (m_plusButton.isHovered)
      return &m_plusButton;
   else if (m_hintButton.isHovered)
      return &m_hintButton;
   else
      return nullptr;
}

#pragma endregion