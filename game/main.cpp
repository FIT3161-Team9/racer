#include <SFML/Graphics.hpp>

#include <engine/engine.h>
#include <engine/timing.h>

#include "game/src/game_state.h"
#include "game/src/gravity.h"
#include "game/src/ground.h"
#include "game/src/ui/main_menu.h"
#include "game/src/ui/result_screen.h"
#include "game/src/ui/splash_screen.h"
#include "game/src/ui/display_course.h"
#include "game/src/ui/vehicle_select.h"
#include "game/src/velocity.h"

int main()
{
  auto app = engine::create_app("RACER");

  engine::run_app(app, [](auto& app_commands) {
    app_commands.template add_resource<GameState>(GameState::CurrentScreen::SplashScreen);
    //app_commands.template add_resource<GameState>(GameState::CurrentVehicle::Vehicle1);

    app_commands.add_plugin(timing::plugin);
    app_commands.add_plugin(velocity::plugin);
    app_commands.add_plugin(gravity::plugin);
    app_commands.add_plugin(ground::circle_ground_collision_plugin);
    app_commands.add_plugin(splash_screen::plugin);
    app_commands.add_plugin(main_menu::plugin);
    app_commands.add_plugin(display_course::plugin);
    app_commands.add_plugin(vehicle_select::plugin);
    app_commands.add_plugin(result_screen::plugin);
  });


  return 0;
}
