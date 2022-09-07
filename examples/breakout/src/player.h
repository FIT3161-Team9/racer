#pragma once

#include <SFML/Graphics.hpp>

#include <engine/app_commands.h>
#include <engine/colour.h>
#include <engine/entity.h>
#include <engine/event.h>
#include <engine/query.h>
#include <engine/rectangle.h>
#include <engine/transform.h>
#include <engine/window.h>
#include <engine/zindex.h>

#include "util.h"
#include "velocity.h"

enum Player {};

namespace player
{
float constexpr SPEED = 1000.f;

inline Entity spawn(AppCommands& app_commands)
{
  return app_commands.spawn()
    .add_component<Transform>(sf::Vector2f{ 0.f, 0.5f * window::COORDINATE_SPACE_HEIGHT - 10 })
    .add_component<Colour>(colour::white())
    .add_component<Rectangle>(sf::Vector2f{ 200.f, 30.f })
    .add_component<Velocity>(util::ZERO_VEC)
    .add_component<ZIndex>(0)
    .add_component<Player>();
}

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system<Event::EventType::KeyPressed>(Query<Player const, Velocity>{}, [&](auto event, auto& view) {
    for (auto [_, __, velocity] : view.each()) {
      if (event.key_pressed.key == sf::Keyboard::Key::Left) { velocity.value.x = -SPEED; }
      if (event.key_pressed.key == sf::Keyboard::Key::Right) { velocity.value.x = SPEED; }
    }
    return false;
  });

  app_commands.add_system<Event::EventType::KeyReleased>(Query<Player const, Velocity>{}, [&](auto event, auto& view) {
    for (auto [_, __, velocity] : view.each()) {
      if (event.key_released.key == sf::Keyboard::Key::Left || event.key_released.key == sf::Keyboard::Key::Right) {
        velocity.value.x = 0;
      }
    }
    return false;
  });

  app_commands.add_system(Query<Player const, Velocity, Transform, Rectangle const>{}, [&](auto& view) {
    for (auto [_, __, velocity, transform, rectangle] : view.each()) {
      if (transform.value.x + 0.5f * rectangle.width_height.x > 0.5f * window::COORDINATE_SPACE_WIDTH) {
        transform.value.x = 0.5f * window::COORDINATE_SPACE_WIDTH - 0.5f * rectangle.width_height.x;
        velocity.value.x = 0.f;
      }
      if (transform.value.x - 0.5f * rectangle.width_height.x < -0.5f * window::COORDINATE_SPACE_WIDTH) {
        transform.value.x = -0.5f * window::COORDINATE_SPACE_WIDTH + 0.5f * rectangle.width_height.x;
        velocity.value.x = 0.f;
      }
    }
  });
}

}// namespace player
