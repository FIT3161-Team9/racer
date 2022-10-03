#pragma once

#include "engine/app_commands.h"
#include "engine/query.h"

#include "engine/vector_utils.h"
#include "game/src/velocity.h"

struct MaximumVelocity
{
  float value;
};

namespace maximum_velocity
{

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system(Query<MaximumVelocity const, Velocity>{}, [&](auto& view) {
    for (auto&& [_, maximum_velocity, velocity] : view.each()) {
      if (vector_utils::magnitude(velocity.value) > maximum_velocity.value) {
        auto const unit_vector =
          vector_utils::scale_vector(velocity.value, 1 / vector_utils::magnitude(velocity.value));
        velocity.value = vector_utils::scale_vector(unit_vector, maximum_velocity.value);
      }
    }
  });
}

}// namespace maximum_velocity
