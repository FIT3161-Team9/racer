#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <iostream>

#include <engine/app_commands.h>
#include <engine/query.h>
#include <engine/timing.h>
#include <engine/transform.h>

/// Component to represent the velocity of an entity
struct Velocity
{
  sf::Vector2f value;
};

namespace velocity
{

Transform apply_velocity(Velocity const& velocity, Transform const& current_transform, std::uint64_t elasped_time);

/// Apply velocity to any entity that has a transform and a velocity. Note: Requires the timing plugin
/// to work
inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system(
    ResourceQuery<timing::ElapsedTime>{}, Query<Transform, Velocity>{}, [](auto& resource_tuple, auto& query) {
      auto&& [_, elapsed_time] = resource_tuple;
      for (auto&& [_, transform, velocity] : query.each()) {
        transform = apply_velocity(velocity, transform, elapsed_time.elapsed_milliseconds);
      }
    });
}

/// Given a velocity, a transform and some amount of time, return a new transform that represents the position
/// an entity would be in if it had been moving from the given transform with the given velocity for the given
/// time
inline Transform
  apply_velocity(Velocity const& velocity, Transform const& current_transform, std::uint64_t elasped_miliseconds)
{
  return Transform{ .value =
                      sf::Vector2f{ current_transform.value.x + velocity.value.x * elasped_miliseconds * 0.001f,
                                    current_transform.value.y + velocity.value.y * elasped_miliseconds * 0.001f } };
}

}// namespace velocity
