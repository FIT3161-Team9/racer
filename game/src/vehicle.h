#pragma once

#include <array>
#include <filesystem>
#include <fstream>
#include <string>

#include "SFML/System/Vector2.hpp"
#include <nlohmann/json.hpp>

#include "engine/app_commands.h"
#include "engine/entity.h"
#include "engine/transform.h"

#include "game/src/camera.h"
#include "game/src/debug/serialization.h"
#include "game/src/utils.h"

struct Vehicle
{
  bool is_player;
};

namespace vehicle
{

std::array<std::string, 2> const VEHICLES = { "./vehicles/Max.json", "./vehicles/Checo.json" };

inline Entity load(AppCommands& app_commands, char const* vehicle_path, sf::Vector2f const location)
{
  nlohmann::json data = utils::parse_json_from_file(vehicle_path);
  auto vehicle = serialization::deserialize_and_spawn(app_commands, data);

  vehicle.add_component<Vehicle>(false).add_component<camera::Target>().add_component<Transform>(location);

  return vehicle;
}

}// namespace vehicle
