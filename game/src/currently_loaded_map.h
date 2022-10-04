#pragma once

#include <string>

#include "SFML/System/Vector2.hpp"

#include "engine/app_commands.h"

struct CurrentlyLoadedMap
{
  std::string name;
  int version;
  sf::Vector2f vehicle_spawn_location;
  std::string preview_path;
};
