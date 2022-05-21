#include <SFML/Graphics.hpp>
#include <iostream>
#include <string_view>

#include "engine/app_commands.h"
#include "engine/circle.h"
#include "engine/engine.h"
#include "engine/transform.h"

int main()
{
  auto app = engine::create_app("RACER");
  engine::run_app(app, [](AppCommands& app_commands) {
    std::cout << "Creating circle\n";
    app_commands.spawn().add_component<Transform>(sf::Vector2f{ 0.f, 0.f }).add_component<Circle>(80.f);
  });

  return 0;
}
