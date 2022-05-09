#include "engine/engine.h"

int main()
{
  auto app = engine::create_app("RACER");

  engine::run_app(app);
  return 0;
}
