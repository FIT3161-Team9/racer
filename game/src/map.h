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
#include "game/src/debug.h"
#include "game/src/debug/deletable.h"
#include "game/src/debug/rotatable.h"
#include "game/src/image_dimensions.h"

struct Map
{
  std::string background_path;
};

namespace map
{

inline void load_level(AppCommands& app_commands, char* level_path)
{
  auto file_path = std::filesystem::path(level_path);
  std::ifstream file(file_path);
  nlohmann::json data = nlohmann::json::parse(file);
  for (auto const& entity : data) { serialization::deserialize_and_spawn(app_commands, entity); }
}

}// namespace map
