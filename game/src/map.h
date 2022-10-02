#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include <SFML/Graphics.hpp>

#include "engine/app_commands.h"
#include "engine/colour.h"
#include "engine/entity.h"
#include "engine/outline.h"
#include "engine/rectangle.h"
#include "engine/rotation.h"
#include "engine/scale.h"
#include "engine/texture.h"
#include "engine/transform.h"
#include "engine/window.h"
#include "engine/zindex.h"

#include "./ground.h"
#include "game/src/currently_loaded_map.h"
#include "game/src/debug/deletable.h"
#include "game/src/debug/rotatable.h"
#include "game/src/debug/serialization.h"
#include "game/src/image_dimensions.h"

namespace map
{

inline CurrentlyLoadedMap load_level(AppCommands& app_commands, char* level_path)
{
  auto file_path = std::filesystem::path(level_path);
  std::ifstream file(file_path);
  nlohmann::json data = nlohmann::json::parse(file);
  for (auto const& entity : data["entities"]) { serialization::deserialize_and_spawn(app_commands, entity); }

  return CurrentlyLoadedMap{ .name = data["name"], .version = data["version"] };
}

}// namespace map
