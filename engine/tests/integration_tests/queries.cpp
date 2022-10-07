#include <gtest/gtest.h>

#include "../../engine.h"
#include "../../query.h"

TEST(Integration, BasicQuery)
{
  enum class TestComponentOne {};
  enum class TestComponentTwo {};
  engine::run_app(engine::create_app("Integration Test"), [](auto& app_commands) {
    app_commands.spawn().template add_component<TestComponentOne>().template add_component<TestComponentTwo>();
    app_commands.spawn().template add_component<TestComponentTwo>();

    app_commands.add_system(Query<TestComponentOne>{}, [&](auto& view) {
      int number_of_entities_matching_query = 0;
      for (auto&& [_, __] : view.each()) { number_of_entities_matching_query += 1; }
      EXPECT_EQ(number_of_entities_matching_query, 1);
    });

    app_commands.add_system(Query<TestComponentOne, TestComponentTwo>{}, [&](auto& view) {
      int number_of_entities_matching_query = 0;
      for (auto&& [_, __, ___] : view.each()) { number_of_entities_matching_query += 1; }
      EXPECT_EQ(number_of_entities_matching_query, 1);
    });

    app_commands.add_system(Query<TestComponentTwo>{}, [&](auto& view) {
      int number_of_entities_matching_query = 0;
      for (auto&& [_, __] : view.each()) { number_of_entities_matching_query += 1; }
      EXPECT_EQ(number_of_entities_matching_query, 2);
      app_commands.quit();
    });
  });
}

TEST(Integration, TwoComponentQueries)
{
  enum class TestComponentOne {};
  enum class TestComponentTwo {};
  engine::run_app(engine::create_app("Integration Test"), [](auto& app_commands) {
    app_commands.spawn().template add_component<TestComponentOne>().template add_component<TestComponentTwo>();
    app_commands.spawn().template add_component<TestComponentTwo>();
    app_commands.spawn().template add_component<TestComponentTwo>();

    app_commands.add_system(Query<TestComponentOne>{}, Query<TestComponentTwo>{}, [&](auto& view_one, auto& view_two) {
      int loop_iterations = 0;
      for (auto&& [_, __] : view_one.each()) { loop_iterations += 1; }

      EXPECT_EQ(loop_iterations, 1);

      for (auto&& [___, ____] : view_two.each()) { loop_iterations += 1; }

      EXPECT_EQ(loop_iterations, 4);

      app_commands.quit();
    });
  });
}

TEST(Integration, ResourceQuery)
{
  struct TestResource
  {
    int value{ 0 };
  };

  engine::run_app(engine::create_app("Integration test"), [](auto& app_commands) {
    app_commands.template add_resource<TestResource>();

    app_commands.add_system(ResourceQuery<TestResource>{}, [&](auto& resources) {
      auto&& [_, test_resource] = resources;
      test_resource.value += 1;
      if (test_resource.value == 5) { app_commands.quit(); }
    });
  });
}
