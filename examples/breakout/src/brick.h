#pragma once

#include <cstdint>

#include <engine/app_commands.h>
#include <engine/colour.h>
#include <engine/entity.h>
#include <engine/rectangle.h>
#include <engine/transform.h>
#include <engine/window.h>

struct Brick
{
  uint32_t health;
};

namespace brick
{

inline Colour color_for_brick_health(uint32_t health)
{
  if (health > 2) { return colour::red(); }
  if (health == 2) { return colour::orange(); }
  return colour::green();
}

std::uint32_t constexpr BRICKS_PER_ROW = 6;
float const GAP = 10.f;
float const WIDTH = (window::COORDINATE_SPACE_WIDTH - GAP * (BRICKS_PER_ROW + 1)) / BRICKS_PER_ROW;
float const HEIGHT = 40.f;

inline Entity spawn(AppCommands& app_commands, std::uint32_t index)
{
  auto const row_number = index / BRICKS_PER_ROW;
  auto const col_number = index % BRICKS_PER_ROW;
  std::uint32_t health = 5 - row_number;
  return app_commands.spawn()
    .add_component<Transform>(sf::Vector2f{
      -0.5f * window::COORDINATE_SPACE_WIDTH + 0.5f * WIDTH + GAP + col_number * WIDTH + col_number * GAP,
      -0.5f * window::COORDINATE_SPACE_HEIGHT + 0.5f * HEIGHT + GAP + row_number * HEIGHT + row_number * GAP })
    .add_component<Brick>(health)
    .add_component<Colour>(color_for_brick_health(health))
    .add_component<Rectangle>(sf::Vector2f{ WIDTH, HEIGHT });
}

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system(Query<Brick>{}, [&](auto& bricks_view) {
    for (auto [brick_entity, brick_component] : bricks_view.each()) {
      if (brick_component.health < 1) { app_commands.destroy(brick_entity); }
    }
  });
  app_commands.add_system(Query<Brick const, Colour>{}, [&](auto& bricks_view) {
    for (auto [_, brick_component, brick_colour] : bricks_view.each()) {
      brick_colour = color_for_brick_health(brick_component.health);
    }
  });
}

};// namespace brick
