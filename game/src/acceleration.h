#pragma once

#include "SFML/System/Vector2.hpp"

#include "engine/app_commands.h"
#include "engine/query.h"
#include "engine/timing.h"
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
    ResourceQuery<timing::ElapsedTime>{}, Query<Acceleration const, Velocity>{}, [&](auto& resources, auto& view) {
      auto&& [_, elapsed_time] = resources;
      auto* pause_state = app_commands.get_resource<debug::pausable::PauseState>();
      if (pause_state != nullptr && pause_state->paused) { return; }
      for (auto&& [_, acceleration, velocity] : view.each()) {
        velocity.value =
          sf::Vector2f{ velocity.value.x + acceleration.value.x * elapsed_time.elapsed_milliseconds * 0.01f,
                        velocity.value.y + acceleration.value.y * elapsed_time.elapsed_milliseconds * 0.01f };
      }
    });
}

}// namespace acceleration
