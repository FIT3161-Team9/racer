#pragma once

/// The global state of the game
struct GameState
{
  /// The current screen to be rendered
  enum class CurrentScreen {
    SplashScreen,
    MainMenu,
    VehicleSelect,
    ResultScreen,
    DisplayCourse,
  };

  CurrentScreen current_screen;
};
