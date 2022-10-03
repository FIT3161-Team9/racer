#pragma once

#include <string>

#include "SFML/System/Vector2.hpp"

#include "engine/app_commands.h"

struct CurrentlyLoadedMap
{
  std::string name;
  int version;
  float finish_line_location;
  sf::Vector2f vehicle_spawn_location;
};
