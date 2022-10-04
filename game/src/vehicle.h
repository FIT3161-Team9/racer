#pragma once

#include <array>
#include <filesystem>
#include <fstream>
#include <string>

#include "SFML/System/Vector2.hpp"
#include <nlohmann/json.hpp>
#include <vector>

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

struct Information
{
  std::string name;
  float speed;
  float acceleration;
  float fuel;
  std::string texture_path;
};

std::array<std::string, 2> const VEHICLES = { "./vehicles/Max.json", "./vehicles/Checo.json" };

inline Entity load(AppCommands& app_commands, char const* vehicle_path, sf::Vector2f const location)
{
  nlohmann::json data = utils::parse_json_from_file(vehicle_path);
  auto vehicle = serialization::deserialize_and_spawn(app_commands, data);

  vehicle.add_component<Vehicle>(false).add_component<camera::Target>().add_component<Transform>(location);

  return vehicle;
}

inline std::vector<Information> load_information()
{
  std::vector<Information> information{};

  for (auto const& vehicle_path : VEHICLES) {
    nlohmann::json data = utils::parse_json_from_file(vehicle_path.c_str());
    information.push_back(Information{ .name = data["name"],
                                       .speed = data["components"]["maximum_velocity"],
                                       .acceleration = data["components"]["acceleration"]["x"],
                                       .fuel = data["components"]["fuel"]["max_capacity"],
                                       .texture_path = data["components"]["texture"] });
  }

  return information;
}

// Given a list of vehicles, return the maximum speed of all the vehicles
inline float max_speed(std::vector<Information> const& vehicles)
{
  float max_speed = 0.f;

  for (auto const& vehicle_information : vehicles) { max_speed = std::max(max_speed, vehicle_information.speed); }

  return max_speed;
}

// Given a list of vehicles, return the minimum speed of all the vehicles
inline float min_speed(std::vector<Information> const& vehicles)
{
  float min_speed = 0.f;

  for (auto const& vehicle_information : vehicles) { min_speed = std::min(min_speed, vehicle_information.speed); }

  return min_speed;
}

// Given a list of vehicles, return the maximum acceleration of all the vehicles
inline float max_acceleration(std::vector<Information> const& vehicles)
{
  float max_acceleration = 0.f;

  for (auto const& vehicle_information : vehicles) {
    max_acceleration = std::max(max_acceleration, vehicle_information.acceleration);
  }

  return max_acceleration;
}

// Given a list of vehicles, return the minimum accleration of all the vehicles
inline float min_acceleration(std::vector<Information> const& vehicles)
{
  float min_acceleration = 0.f;

  for (auto const& vehicle_information : vehicles) {
    min_acceleration = std::min(min_acceleration, vehicle_information.acceleration);
  }

  return min_acceleration;
}
//
// Given a list of vehicles, return the maximum fuel of all the vehicles
inline float max_fuel(std::vector<Information> const& vehicles)
{
  float max_fuel = 0.f;

  for (auto const& vehicle_information : vehicles) { max_fuel = std::max(max_fuel, vehicle_information.fuel); }

  return max_fuel;
}

// Given a list of vehicles, return the minimum fuel of all the vehicles
inline float min_fuel(std::vector<Information> const& vehicles)
{
  float min_fuel = 0.f;

  for (auto const& vehicle_information : vehicles) { min_fuel = std::min(min_fuel, vehicle_information.fuel); }

  return min_fuel;
}


}// namespace vehicle
