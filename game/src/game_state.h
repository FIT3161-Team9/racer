#pragma once

/// The global state of the game
struct GameState
{
  /// The current screen to be rendered
  enum class CurrentScreen { SplashScreen, MainMenu, VehicleSelect1, VehicleSelect2, ResultScreen, DisplayCourse, InLevel };

  enum class CurrentMap {Map1, Map2};

  enum class CurrentVehicle_p1 {Vehicle1, Vehicle2, Vehicle3};

  enum class CurrentVehicle_p2 {Vehicle1, Vehicle2, Vehicle3};

  CurrentScreen current_screen;

  CurrentMap current_map;

  CurrentVehicle_p1 current_vehicle_p1;

  CurrentVehicle_p2 current_vehicle_p2;
};
