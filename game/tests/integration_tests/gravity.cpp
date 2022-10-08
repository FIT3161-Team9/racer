#include "SFML/System/Vector2.hpp"
#include <gtest/gtest.h>

#include "engine/app_commands.h"
#include "engine/engine.h"
#include "engine/query.h"
#include "engine/timing.h"

#include "engine/transform.h"
#include "game/src/gravity.h"
#include "game/src/velocity.h"

TEST(Integration, Gravity)
{
  auto const INITIAL_VELOCITY = sf::Vector2f{ 0.f, 0.f };
  auto updated_velocity = INITIAL_VELOCITY;

  auto iterations = 0;

  engine::run_app(engine::create_app("Gravity Integration Test"), [&](auto& app_commands) {
    app_commands.add_plugin(timing::plugin);
    app_commands.add_plugin(gravity::plugin);

    app_commands.spawn().template add_component<Velocity>(INITIAL_VELOCITY).template add_component<AffectedByGravity>();

    app_commands.add_system(Query<Velocity>{}, [&](auto& view) {
      for (auto&& [_, velocity] : view.each()) {
        EXPECT_TRUE(velocity.value.y >= updated_velocity.y);
        EXPECT_EQ(velocity.value.x, updated_velocity.x);
        updated_velocity = velocity.value;
      }
      iterations += 1;
      if (iterations > 10) { app_commands.quit(); }
    });
  });

  EXPECT_TRUE(updated_velocity.y > INITIAL_VELOCITY.y);
  EXPECT_EQ(updated_velocity.x, INITIAL_VELOCITY.x);
}

TEST(Integration, GravityTransform)
{
  auto const INITIAL_VELOCITY = sf::Vector2f{ 0.f, 0.f };
  auto const INITIAL_TRANSFORM = sf::Vector2f{ 0.f, 0.f };
  auto updated_transform = INITIAL_TRANSFORM;

  auto iterations = 0;

  engine::run_app(engine::create_app("Gravity Integration Test"), [&](auto& app_commands) {
    app_commands.add_plugin(timing::plugin);
    app_commands.add_plugin(velocity::plugin);
    app_commands.add_plugin(gravity::plugin);

    app_commands.spawn()
      .template add_component<Transform>(INITIAL_TRANSFORM)
      .template add_component<Velocity>(INITIAL_VELOCITY)
      .template add_component<AffectedByGravity>();

    app_commands.add_system(Query<Transform>{}, [&](auto& view) {
      for (auto&& [_, transform] : view.each()) { updated_transform = transform.value; }
      iterations += 1;
      if (iterations > 10) { app_commands.quit(); }
    });
  });

  EXPECT_TRUE(updated_transform.y > INITIAL_TRANSFORM.y);
  EXPECT_EQ(updated_transform.x, INITIAL_TRANSFORM.x);
}
