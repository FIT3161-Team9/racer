#include <gtest/gtest.h>

#include "../../engine.h"
#include "../../query.h"

TEST(Integration, StartupFunctionRuns)
{
  auto const app = engine::create_app("Integration Test");

  auto startup_function_ran = false;

  engine::run_app(app, [&startup_function_ran](auto& app_commands) {
    startup_function_ran = true;
    app_commands.quit();
  });

  EXPECT_TRUE(startup_function_ran);
}

TEST(Integration, SystemRuns)
{
  int times_ran = 0;

  engine::run_app(engine::create_app("Integration Test"), [&times_ran](auto& app_commands) {
    app_commands.add_system(Query<int>{}, [&](auto& view) {
      (void)view;
      if (times_ran == 4) {
        app_commands.quit();
        return;
      }
      times_ran += 1;
    });
  });

  EXPECT_EQ(times_ran, 4);
}
