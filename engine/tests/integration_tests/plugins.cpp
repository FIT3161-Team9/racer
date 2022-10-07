#include <gtest/gtest.h>

#include "../../engine.h"
#include "../../query.h"

TEST(Integration, Plugin)
{
  bool plugin_system_did_run = false;

  auto const test_plugin = [&](auto& app_commands) {
    app_commands.add_system(Query<int>{}, [&](auto& view) {
      (void)view;
      plugin_system_did_run = true;
      app_commands.quit();
    });
  };

  engine::run_app(engine::create_app("Integration Test"),
                  [&test_plugin](auto& app_commands) { app_commands.add_plugin(test_plugin); });

  EXPECT_TRUE(plugin_system_did_run);
}
