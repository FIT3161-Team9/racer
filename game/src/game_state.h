#pragma once

#include "game/src/map.h"
#include <cstdlib>
#include <ctime>

/// The global state of the game

struct GameState
{
  /// The current screen to be rendered
  enum class CurrentScreen {
    SplashScreen,
    MainMenu,
    VehicleSelectPlayerOne,
    VehicleSelectPlayerTwo,
    MapSelect,
    InLevel,
    ResultScreen,
  };

  struct SplashScreen
  {
  };

  struct MainMenu
  {
  };

  struct VehicleSelectPlayerOne
  {
    int selected_map_index;
  };

  struct VehicleSelectPlayerTwo
  {
    int selected_map_index;
    int player_one_vehicle;
  };

  struct MapSelect
  {
    int selected_map_index;
  };

  struct InLevel
  {
  };

  struct ResultScreen
  {
    float milliseconds_to_complete;
    bool player_one_did_win;
  };

  CurrentScreen current_screen{};

  union {
    SplashScreen splash_screen;
    MainMenu main_menu;
    VehicleSelectPlayerOne vehicle_select_player_one;
    VehicleSelectPlayerTwo vehicle_select_player_two;
    MapSelect map_select;
    InLevel in_level;
    ResultScreen result_screen;
  };
};

namespace game_state
{

inline GameState vehicle_select_player_two(int selected_map_index, int vehicle_index)
{
  return GameState{ .current_screen = GameState::CurrentScreen::VehicleSelectPlayerTwo,
                    .vehicle_select_player_two = GameState::VehicleSelectPlayerTwo{
                      .selected_map_index = selected_map_index, .player_one_vehicle = vehicle_index } };
}

inline GameState vehicle_select_player_one(int selected_map_index)
{
  return GameState{ .current_screen = GameState::CurrentScreen::VehicleSelectPlayerOne,
                    .vehicle_select_player_one =
                      GameState::VehicleSelectPlayerOne{ .selected_map_index = selected_map_index } };
}

inline GameState splash_screen()
{
  return GameState{ .current_screen = GameState::CurrentScreen::SplashScreen,
                    .splash_screen = GameState::SplashScreen() };
}

inline GameState level_finished(float milliseconds, bool player_one_did_win)
{
  return GameState{ .current_screen = GameState::CurrentScreen::ResultScreen,
                    .result_screen = GameState::ResultScreen{ .milliseconds_to_complete = milliseconds,
                                                              .player_one_did_win = player_one_did_win } };
}

inline GameState map_select()
{
  std::srand(std::time(nullptr));
  int const random_map_index = std::rand() % map::LEVELS.size();

  return GameState{ .current_screen = GameState::CurrentScreen::MapSelect,
                    .map_select = GameState::MapSelect{ .selected_map_index = random_map_index } };
}

}// namespace game_state
