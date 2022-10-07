#include <gtest/gtest.h>

#include "../../engine.h"

TEST(Integration, SpawnEntity)
{
  engine::run_app(engine::create_app("Integration Test"), [](auto& app_commands) {
    app_commands.spawn();
    app_commands.quit();
  });
}

TEST(Integration, AddComponentToEntity)
{
  enum class TestComponent {};

  engine::run_app(engine::create_app("Integration Test"), [](auto& app_commands) {
    auto const entity = app_commands.spawn().template add_component<TestComponent>();

    auto const* test_component = app_commands.template component<TestComponent>(entity.entity());

    EXPECT_TRUE(test_component != nullptr);

    app_commands.quit();
  });
}
