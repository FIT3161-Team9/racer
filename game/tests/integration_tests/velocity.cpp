#include <gtest/gtest.h>

#include "SFML/System/Vector2.hpp"
#include "engine/app_commands.h"
#include "engine/engine.h"
#include "engine/query.h"
#include "engine/timing.h"
#include "engine/transform.h"
#include "engine/vector_utils.h"

#include "game/src/vehicle.h"
#include "game/src/velocity.h"

TEST(Integration, Velocity)
{
  auto const INITIAL_TRANSFORM = sf::Vector2f{ 0.f, 0.f };
  int number_of_iterations = 0;
  auto last_transform = INITIAL_TRANSFORM;

  engine::run_app(engine::create_app("Integration test"), [&](auto& app_commands) {
    app_commands.add_plugin(timing::plugin);
    app_commands.add_plugin(velocity::plugin);

    app_commands.spawn()
      .template add_component<Transform>(INITIAL_TRANSFORM)
      .template add_component<Velocity>(sf::Vector2f{ 10.f, 0.f });

    app_commands.add_system(Query<Transform const>{}, [&](auto& view) {
      for (auto&& [_entity, transform] : view.each()) {
        EXPECT_TRUE(transform.value.x >= last_transform.x);
        last_transform = transform.value;
      }
      number_of_iterations += 1;
      if (number_of_iterations == 10) { app_commands.quit(); }
    });
  });

  EXPECT_TRUE(last_transform.x > INITIAL_TRANSFORM.x);
  EXPECT_TRUE(last_transform.y == INITIAL_TRANSFORM.y);
}
