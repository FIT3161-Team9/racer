#pragma once

#include "engine/app_commands.h"
#include "engine/query.h"
#include "engine/timing.h"
#include "game/src/velocity.h"

enum class AffectedByGravity {};

namespace gravity
{

float constexpr GRAVITY = 9.8f;

inline void plugin(AppCommands& app_commands)
{

  app_commands.add_system(
    ResourceQuery<timing::ElapsedTime>{}, Query<AffectedByGravity, Velocity>{}, [](auto& resources, auto& view) {
      auto&& [_, elapsed_time] = resources;

      for (auto&& [_entity, _affected_by_gravity, velocity] : view.each()) {
        velocity.value.y += elapsed_time.elapsed_milliseconds * GRAVITY;
      }
    });
}

}// namespace gravity
