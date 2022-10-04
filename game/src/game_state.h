#pragma once

/// The global state of the game
struct GameState
{
  /// The current screen to be rendered
  enum class CurrentScreen { SplashScreen, MainMenu, VehicleSelect1, VehicleSelect2, ResultScreen, DisplayCourse, InLevel };

  enum class CurrentMap {Map1, Map2};

  enum class CurrentVehicle {Vehicle1, Vehicle2, Vehicle3};

  CurrentScreen current_screen;

  CurrentMap current_map;

  CurrentVehicle current_vehicle;
};
