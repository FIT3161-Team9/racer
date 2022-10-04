#pragma once

#include "SFML/System/Vector2.hpp"

#include "engine/app_commands.h"
#include "engine/query.h"
#include "engine/timing.h"

#include "game/src/fuel.h"
#include "game/src/pause_state.h"
#include "game/src/velocity.h"

struct Acceleration
{
  sf::Vector2f value;
};

namespace acceleration
{

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system(
    ResourceQuery<timing::ElapsedTime>{},
    Query<Acceleration const, Velocity, Fuel>{},
    [&](auto& resources, auto& view) {
      auto&& [_, elapsed_time] = resources;
      auto* pause_state = app_commands.get_resource<PauseState>();
      if (pause_state != nullptr && pause_state->paused) { return; }
      for (auto&& [_, acceleration, velocity, fuel] : view.each()) {
        if (fuel.current_fuel > 0) {
          velocity.value =
            sf::Vector2f{ velocity.value.x + acceleration.value.x * elapsed_time.elapsed_milliseconds * 0.01f,
                          velocity.value.y + acceleration.value.y * elapsed_time.elapsed_milliseconds * 0.01f };
          fuel.current_fuel -= fuel.burn_rate * elapsed_time.elapsed_milliseconds * 0.01f;
        }
      }
    });
}

}// namespace acceleration
