#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <iostream>
#include <numbers>

#include <engine/app_commands.h>
#include <engine/event.h>
#include <engine/query.h>
#include <engine/timing.h>
#include <engine/transform.h>
#include <engine/window.h>

#include "ball.h"
#include "brick.h"
#include "player.h"
#include "velocity.h"

namespace physics
{

auto const ball_player_collision = [](auto& player_view, auto& ball_view) {
  auto ball_tuple = *ball_view.each().begin();
  auto& ball_radius = std::get<3>(ball_tuple).radius;
  auto ball_y = std::get<1>(ball_tuple).value.y;
  auto ball_x = std::get<1>(ball_tuple).value.x;
  auto& ball_velocity = std::get<2>(ball_tuple);

  auto player_tuple = *player_view.each().begin();
  auto player_height = std::get<2>(player_tuple).width_height.y;
  auto player_width = std::get<2>(player_tuple).width_height.x;
  auto player_y = std::get<1>(player_tuple).value.y;
  auto player_x = std::get<1>(player_tuple).value.x;

  if (ball_y + ball_radius >= player_y - 0.5f * player_height && ball_velocity.value.y > 0) {
    if (ball_x > player_x - 0.5f * player_width && ball_x < player_x + 0.5f * player_width) {
      ball_velocity.value.y = -ball_velocity.value.y;

      // rotate vector by difference between ball and center of player
      auto difference_between_ball_and_player = ((ball_x - player_x) / 2) * (std::numbers::pi_v<float> / 180);

      auto original_ball_velocity_x = ball_velocity.value.x;
      ball_velocity.value.x = ball_velocity.value.x * std::cos(difference_between_ball_and_player)
                              - ball_velocity.value.y * std::sin(difference_between_ball_and_player);
      ball_velocity.value.y = original_ball_velocity_x * std::sin(difference_between_ball_and_player)
                              + ball_velocity.value.y * std::cos(difference_between_ball_and_player);
    }
  }
};

auto const ball_wall_collision = [](auto& ball_view) {
  auto ball_tuple = *ball_view.each().begin();
  auto& ball_transform = std::get<1>(ball_tuple).value;
  auto& ball_velocity = std::get<2>(ball_tuple).value;
  auto ball_radius = std::get<3>(ball_tuple).radius;

  if ((ball_transform.x + ball_radius > 0.5f * window::COORDINATE_SPACE_WIDTH && ball_velocity.x > 0)
      || (ball_transform.x - ball_radius < -0.5f * window::COORDINATE_SPACE_WIDTH && ball_velocity.x < 0)) {
    ball_velocity.x = -ball_velocity.x;
  }
  if (ball_transform.y - ball_radius < -0.5f * window::COORDINATE_SPACE_HEIGHT && ball_velocity.y < 0) {
    ball_velocity.y = -ball_velocity.y;
  }
};

inline auto ball_brick_collision = [](auto& bricks_view, auto& ball_view) {
  auto ball_tuple = *ball_view.each().begin();
  auto& ball_velocity = std::get<2>(ball_tuple).value;
  auto const& ball_transform = std::get<1>(ball_tuple).value;
  auto ball_radius = std::get<3>(ball_tuple).radius;

  for (auto [_, brick_transform, brick_rectangle, brick] : bricks_view.each()) {
    if (std::abs(brick_transform.value.y + 0.5f * brick_rectangle.width_height.y - ball_transform.y) < ball_radius
        && ball_transform.x > brick_transform.value.x - 0.5f * brick_rectangle.width_height.x
        && ball_transform.x < brick_transform.value.x + 0.5f * brick_rectangle.width_height.x && ball_velocity.y < 0) {
      brick.health -= 1;
      ball_velocity.y = -ball_velocity.y;
      if (util::magnitude(ball_velocity) < ball::MAX_SPEED) {
        ball_velocity.x *= 1.1;
        ball_velocity.y *= 1.1;
      }
    }
  }
};

inline void plugin(AppCommands& app_commands)
{
  // Update transform based on velocity
  app_commands.add_system(
    ResourceQuery<timing::ElapsedTime>{}, Query<Transform, Velocity const>{}, [](auto& resources, auto& view) {
      auto elapsed_milliseconds = std::get<1>(resources).elapsed_milliseconds;
      for (auto [_, transform, velocity] : view.each()) {
        transform.value.x += static_cast<float>(velocity.value.x * elapsed_milliseconds * 0.001f);
        transform.value.y += static_cast<float>(velocity.value.y * elapsed_milliseconds * 0.001f);
      }
    });

  app_commands.add_system(Query<Transform, Rectangle, Player const>{},
                          Query<Transform, Velocity, Circle, Ball const>{},
                          ball_player_collision);

  app_commands.add_system(Query<Transform, Velocity, Circle, Ball const>{}, ball_wall_collision);

  app_commands.add_system(Query<Transform const, Rectangle, Brick>{},
                          Query<Transform const, Velocity, Circle, Ball const>{},
                          ball_brick_collision);
}

}// namespace physics
