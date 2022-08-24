#pragma once

#include "engine/app_commands.h"
#include "engine/query.h"
#include "engine/rotation.h"
#include "engine/timing.h"


struct RotationalVelocity
{
  float value;
};

namespace rotational_velocity
{

void plugin(AppCommands& app_commands)
{
  app_commands.add_system(
    ResourceQuery<timing::ElapsedTime>{}, Query<RotationalVelocity, Rotation>{}, [](auto& resource_tuple, auto& view) {
      auto&& [_, elapsed_time] = resource_tuple;
      for (auto&& [_, rotational_velocity, rotation] : view.each()) {
        rotation.degrees += rotational_velocity.value * elapsed_time.elapsed_milliseconds * 0.001f;
      }
    });
}

}// namespace rotational_velocity
