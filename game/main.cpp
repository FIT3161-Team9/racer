#include <iostream>

#include <SFML/Graphics.hpp>

#include <engine/engine.h>
#include <engine/timing.h>

#include "game/src/acceleration.h"
#include "game/src/arguments.h"
#include "game/src/camera.h"
#include "game/src/count_down_timer.h"
#include "game/src/cross_line.h"
#include "game/src/currently_loaded_map.h"
#include "game/src/debug.h"
#include "game/src/game_state.h"
#include "game/src/gravity.h"
#include "game/src/ground.h"
#include "game/src/image_dimensions.h"
#include "game/src/map.h"
#include "game/src/maximum_velocity.h"
#include "game/src/pause_state.h"
#include "game/src/shift_key_resource.h"
#include "game/src/ui/main_menu.h"
#include "game/src/ui/map_select.h"
#include "game/src/ui/result_screen.h"
#include "game/src/ui/splash_screen.h"
#include "game/src/ui/vehicle_select.h"
#include "game/src/velocity.h"

int main(int argc, char** argv)
{
  auto arguments = arguments::parse(argc, argv);

  auto app = engine::create_app("RACER");

  engine::run_app(app, [arguments](auto& app_commands) {
    app_commands.template add_resource<GameState>(game_state::splash_screen());
    app_commands.template add_resource<PauseState>(true);

    app_commands.add_plugin(count_down_timer::plugin);
    app_commands.add_plugin(timing::plugin);
    app_commands.add_plugin(velocity::plugin);
    app_commands.add_plugin(acceleration::plugin);
    app_commands.add_plugin(gravity::plugin);
    app_commands.add_plugin(ground::circle_ground_collision_plugin);
    app_commands.add_plugin(camera::plugin);
    app_commands.add_plugin(image_dimensions::synchronise_image_size_with_rect_size_plugin);
    app_commands.add_plugin(shift_key_resource::plugin);
    app_commands.add_plugin(arrow_keys_resource::plugin);
    app_commands.add_plugin(maximum_velocity::plugin);
    app_commands.add_plugin(cross_line::plugin);

    if (arguments.debug_mode) {
      std::cout << "Loading in debug mode\n";
      app_commands.add_plugin(debug::plugin);
    }
    if (arguments.level != nullptr) {
      std::cout << "Loading level " << arguments.level << "\n";
      auto const map = map::load_level(app_commands, arguments.level);
      app_commands.template add_resource<CurrentlyLoadedMap>(map);
    }

    if (arguments.level == nullptr) {
      app_commands.add_plugin(splash_screen::plugin);
      app_commands.add_plugin(main_menu::plugin);
      app_commands.add_plugin(vehicle_select::plugin);
      app_commands.add_plugin(result_screen::plugin);
      app_commands.add_plugin(map_select::plugin);
    }
  });

  return 0;
}
