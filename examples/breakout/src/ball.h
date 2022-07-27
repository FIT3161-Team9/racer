#pragma once

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

#include <engine/app_commands.h>
#include <engine/circle.h>
#include <engine/colour.h>
#include <engine/query.h>
#include <engine/transform.h>
#include <engine/window.h>

#include "util.h"
#include "velocity.h"

enum Ball {};

namespace ball
{

float const STARTING_SPEED = 1000.f;
float const MAX_SPEED = 1600.f;

inline sf::Vector2f ball_starting_velocity()
{
  const std::int32_t seed = std::random_device{}();
  std::mt19937 generator(seed);
  std::uniform_int_distribution<std::int32_t> distribute(-STARTING_SPEED / 2, STARTING_SPEED / 2);

  auto const random_value = distribute(generator);

  float x_velocity = random_value;
  float y_velocity = std::sqrt(STARTING_SPEED * STARTING_SPEED - x_velocity * x_velocity);
  return sf::Vector2f{ x_velocity, y_velocity };
}

inline Entity spawn(AppCommands& app_commands)
{
  return app_commands.spawn()
    .add_component<Transform>(util::ZERO_VEC)
    .add_component<Colour>(colour::white())
    .add_component<Circle>(20.f)
    .add_component<Velocity>(ball_starting_velocity())
    .add_component<Ball>();
}

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system(Query<Transform, Circle, Ball const>{}, [&](auto& ball_view) {
    auto ball_tuple = *ball_view.each().begin();
    auto& ball_transform = std::get<1>(ball_tuple).value;
    auto ball_radius = std::get<2>(ball_tuple).radius;
    auto ball_entity = std::get<0>(ball_tuple);
    if (ball_transform.y - ball_radius > 0.5f * window::COORDINATE_SPACE_HEIGHT) {
      app_commands.destroy(ball_entity);
      spawn(app_commands);
    }
  });
}

};// namespace ball
