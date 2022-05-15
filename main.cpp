#include <SFML/Graphics.hpp>
#include <iostream>
#include <string_view>

#include "engine/app_commands.h"
#include "engine/circle.h"
#include "engine/engine.h"

struct SayHello
{
  std::string_view name;
};

int main()
{
  auto app = engine::create_app("RACER");

  app.on_startup = [](AppCommands& app_commands) -> void {
    app_commands.spawn().add_component<SayHello>("James");

    app_commands.add_system<SayHello const>(
      [](auto& view) { view.each([](auto& say_hello) { std::cout << say_hello.name << " says hello!\n"; }); });
  };

  engine::run_app(app);
  return 0;
}
