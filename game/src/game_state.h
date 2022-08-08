#pragma once

struct GameState
{
  enum class CurrentScreen {
    SplashScreen,
    MainMenu,
  };

  CurrentScreen current_screen;
};
