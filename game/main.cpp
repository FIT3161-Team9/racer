#include <SFML/Graphics.hpp>

#include <engine/engine.h>
#include <engine/timing.h>

#include "game/src/car.h"
#include "game/src/game_state.h"
#include "game/src/gravity.h"
#include "game/src/ground.h"
#include "game/src/ui/splash_screen.h"
#include "game/src/velocity.h"


int main()
{
  auto app = engine::create_app("RACER");

  engine::run_app(app, [](auto& app_commands) {
    app_commands.template add_resource<GameState>(GameState::CurrentScreen::SplashScreen);

    app_commands.add_plugin(timing::plugin);
    app_commands.add_plugin(velocity::plugin);
    app_commands.add_plugin(car::plugin);
    app_commands.add_plugin(gravity::plugin);
    app_commands.add_plugin(ground::circle_ground_collision_plugin);
    app_commands.add_plugin(splash_screen::plugin);
    app_commands.add_plugin(main_menu::plugin);
  });


  return 0;
}
