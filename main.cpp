#include <SFML/Graphics.hpp>
#include <iostream>

#include "engine/circle.h"
#include "engine/engine.h"

int main()
{
  auto app = engine::create_app("RACER");

  app.on_startup = [](auto&) -> void { std::cout << "Hello World!\n"; };

  engine::run_app(app);
  return 0;
}
