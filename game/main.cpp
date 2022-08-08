#include <SFML/Graphics.hpp>

#include <engine/engine.h>

#include "./src/game_state.h"
#include "./src/splash_screen.h"

int main()
{
  auto app = engine::create_app("RACER");

  engine::run_app(app, [](auto& app_commands) {
    app_commands.template add_resource<GameState>(GameState::CurrentScreen::SplashScreen);

    app_commands.add_plugin(splash_screen::plugin);
  });


  return 0;
}
