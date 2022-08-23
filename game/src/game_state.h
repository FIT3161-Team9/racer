#pragma once

/// The global state of the game
struct GameState
{
  /// The current screen to be rendered
  enum class CurrentScreen {
    SplashScreen,
    MainMenu,
    DisplayCrouse,
  };

  CurrentScreen current_screen;
};
