#include <SFML/Graphics.hpp>

#include "engine/circle.h"
#include "engine/engine.h"

int main()
{
  auto app = engine::create_app("RACER");

  engine::run_app(app);

  return 0;
}
