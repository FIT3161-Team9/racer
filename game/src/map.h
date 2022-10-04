#pragma once

#include <array>
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

#include "game/src/currently_loaded_map.h"
#include "game/src/debug/deletable.h"
#include "game/src/debug/rotatable.h"
#include "game/src/debug/serialization.h"
#include "game/src/debug/spawn_vehicle.h"
#include "game/src/finish_line.h"
#include "game/src/ground.h"
#include "game/src/image_dimensions.h"
#include "game/src/utils.h"

namespace map
{

std::array<std::string, 3> const LEVELS = { "./levels/Grass Plains.json",
                                            "./levels/Highway.json",
                                            "./levels/Mountains.json" };

struct Information
{
  std::string name;
  std::string preview_file_path;
};

inline CurrentlyLoadedMap load_level(AppCommands& app_commands, char const* level_path)
{
  auto const data = utils::parse_json_from_file(level_path);
  for (auto const& entity : data["entities"]) { serialization::deserialize_and_spawn(app_commands, entity); }

  return CurrentlyLoadedMap{ .name = data["name"],
                             .version = data["version"],
                             .vehicle_spawn_location =
                               serialization::deserialize_vector(data["vehicle_spawn_location"]),
                             .preview_path = data["preview"] };
}

inline std::vector<Information> load_information()
{
  std::vector<Information> information{};

  for (auto const& level_path : LEVELS) {
    auto const json = utils::parse_json_from_file(level_path.c_str());
    information.push_back(Information{ .name = json["name"], .preview_file_path = json["preview"] });
  }

  return information;
}

}// namespace map
