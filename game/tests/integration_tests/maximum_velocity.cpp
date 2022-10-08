#include "SFML/System/Vector2.hpp"
#include <gtest/gtest.h>

#include "engine/app_commands.h"
#include "engine/engine.h"
#include "engine/query.h"
#include "engine/timing.h"
#include "engine/vector_utils.h"

#include "game/src/acceleration.h"
#include "game/src/fuel.h"
#include "game/src/maximum_velocity.h"
#include "game/src/velocity.h"

TEST(Integration, MaximumVelocity)
{
  auto const INITIAL_VELOCITY = sf::Vector2f{ 0.f, 0.f };
  auto const MAXIMUM_VELOCITY = 1.f;
  int iterations = 0;
  auto current_velocity = INITIAL_VELOCITY;

  engine::run_app(engine::create_app("Maximum Velocity Integration Test"), [&](auto& app_commands) {
    app_commands.add_plugin(timing::plugin);
    app_commands.add_plugin(acceleration::plugin);
    app_commands.add_plugin(maximum_velocity::plugin);

    app_commands.spawn()
      .template add_component<Velocity>(INITIAL_VELOCITY)
      .template add_component<Acceleration>(sf::Vector2f{ 1000000.f, 0.f })
      .template add_component<MaximumVelocity>(MAXIMUM_VELOCITY)
      .template add_component<Fuel>(0.f, 1.f, 1.f);

    app_commands.add_system(Query<Velocity>{}, [&](auto& view) {
      for (auto&& [_, velocity] : view.each()) { current_velocity = velocity.value; }
      iterations += 1;
      if (iterations > 100) { app_commands.quit(); }
    });
  });

  EXPECT_TRUE(current_velocity.x == MAXIMUM_VELOCITY);
}
