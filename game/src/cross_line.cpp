#include <iostream>

#include <SFML/Graphics.hpp>

#include "game/src/cross_line.h"
#include "game/src/game_state.h"
#include "game/src/ui/result_screen.h"
#include "game/src/vehicle.h"

namespace cross_line
{

void start_timer(AppCommands& app_commands)
{
  auto& timer = *app_commands.get_resource<Timer>();
  timer.value.reset();
  timer.value = sf::Clock();
  timer.value.value().restart();
}


void plugin(AppCommands& app_commands)
{
  app_commands.add_resource<Timer>();
  app_commands.add_system(
    ResourceQuery<GameState, PauseState, Timer>{},
    Query<FinishLine const, Transform const>{},
    Query<Vehicle const, Transform const>{},
    [&](auto& resources, auto& finish_line_view, auto& vehicle_view) {
      auto&& [_, game_state, pause_state, timer] = resources;
      for (auto&& [finish_line_entity, _finish_line, finish_line_transform] : finish_line_view.each()) {
        for (auto&& [vehicle_entity, vehicle, vehicle_transform] : vehicle_view.each()) {
          if (vehicle_transform.value.x >= finish_line_transform.value.x) {
            pause_state.paused = true;
            std::cout << "Race took " << timer.value.value().getElapsedTime().asSeconds() << " seconds\n";
            game_state.current_screen = GameState::CurrentScreen::ResultScreen;
            app_commands.destroy_all();
            result_screen::spawn_ui(app_commands);
          }
        }
      }
    });
}

}// namespace cross_line
