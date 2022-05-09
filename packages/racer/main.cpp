#include <SFML/Graphics.hpp>

#include "engine/circle.h"
#include "engine/engine.h"

int main()
{
  auto app = engine::create_app("RACER");

  engine::spawn_entity<Circle>(app, sf::Vector2f{ 0.0f, 0.0f }, 100);

  engine::run_app(app);

  return 0;
}
