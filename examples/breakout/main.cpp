#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdint>
#include <entt/entt.hpp>
#include <functional>
#include <iostream>
#include <optional>
#include <string_view>

#include <engine/app_commands.h>
#include <engine/circle.h>
#include <engine/colour.h>
#include <engine/engine.h>
#include <engine/event.h>
#include <engine/query.h>
#include <engine/rectangle.h>
#include <engine/timing.h>
#include <engine/transform.h>

#include "src/ball.h"
#include "src/brick.h"
#include "src/drag_and_drop_circles.h"
#include "src/gravity.h"
#include "src/physics.h"
#include "src/player.h"
#include "src/velocity.h"

int main()
{
  auto app = engine::create_app("Breakout");

  engine::run_app(app, [](AppCommands& app_commands) {
    app_commands.add_plugin(timing::plugin);
    app_commands.add_plugin(player::plugin);
    app_commands.add_plugin(ball::plugin);
    app_commands.add_plugin(brick::plugin);
    app_commands.add_plugin(physics::plugin);
    player::spawn(app_commands);
    ball::spawn(app_commands);
    for (std::uint32_t i = 0; i < brick::BRICKS_PER_ROW * 5; i++) { brick::spawn(app_commands, i); }
  });

  return 0;
}
