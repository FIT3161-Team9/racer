#pragma once

#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "SFML/Window/Keyboard.hpp"
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "SFML/Window/WindowStyle.hpp"
#include "engine/app_commands.h"
#include "engine/circle.h"
#include "engine/colour.h"
#include "engine/entity.h"
#include "engine/event.h"
#include "engine/outline.h"
#include "engine/query.h"
#include "engine/rotation.h"
#include "engine/scale.h"
#include "engine/texture.h"
#include "engine/transform.h"
#include "engine/zindex.h"

#include "../ground.h"
#include "../image_dimensions.h"
#include "game/src/acceleration.h"
#include "game/src/camera.h"
#include "game/src/currently_loaded_map.h"
#include "game/src/debug/deletable.h"
#include "game/src/debug/draggable.h"
#include "game/src/debug/resizeable.h"
#include "game/src/debug/rotatable.h"
#include "game/src/debug/selectable.h"
#include "game/src/finish_line.h"
#include "game/src/fuel.h"
#include "game/src/gravity.h"
#include "game/src/maximum_velocity.h"
#include "game/src/spawn_transform.h"
#include "game/src/velocity.h"

namespace serialization
{

enum class Serialized {};

nlohmann::json serialize(AppCommands& app_commands, entt::entity entity);
nlohmann::json serialize_outline(Outline const&);
nlohmann::json serialize_colour(Colour const&);
nlohmann::json serialize_vector(sf::Vector2f const&);
nlohmann::json serialize_fuel(Fuel const&);

Fuel deserialize_fuel(nlohmann::json const&);
sf::Vector2f deserialize_vector(nlohmann::json const&);
Outline deserialize_outline(nlohmann::json const&);
Colour deserialize_colour(nlohmann::json const&);
std::string get_level_file_name(CurrentlyLoadedMap*);

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system<Event::EventType::KeyReleased>(Query<Serialized>{}, [&](auto& event, auto& view) {
    if (event.key_released.key != sf::Keyboard::D) { return false; }

    auto* loaded_level = app_commands.get_resource<CurrentlyLoadedMap>();

    auto file_name = get_level_file_name(loaded_level);
    std::ofstream outfile(std::filesystem::path(std::string("./levels/") + file_name));

    nlohmann::json map_data;

    nlohmann::json entity_array;

    for (auto&& [entity, _serialized] : view.each()) { entity_array.push_back(serialize(app_commands, entity)); }

    map_data["entities"] = entity_array;
    map_data["name"] = loaded_level == nullptr ? "Unnamed Level" : loaded_level->name;
    map_data["version"] = loaded_level == nullptr ? 0 : loaded_level->version + 1;
    map_data["vehicle_spawn_location"] =
      serialize_vector(loaded_level == nullptr ? sf::Vector2f{ 0.f, 0.f } : loaded_level->vehicle_spawn_location);

    outfile << map_data.dump();

    outfile.close();

    return true;
  });
}

inline std::string get_level_file_name(CurrentlyLoadedMap* currently_loaded_map)
{
  if (currently_loaded_map != nullptr) {
    return currently_loaded_map->name + " (" + std::to_string(currently_loaded_map->version + 1) + ").json";
  }
  auto time = std::time(nullptr);
  char file_name[100];
  std::strftime(file_name, sizeof(file_name), "%Y-%m-%d_%I.%M.%S.json", std::localtime(&time));
  return file_name;
}

inline nlohmann::json serialize(AppCommands& app_commands, entt::entity entity)
{
  using json = nlohmann::json;

  json components;

  auto const* texture = app_commands.component<Texture>(entity);
  auto const* image_dimensions = app_commands.component<ImageDimensions>(entity);
  auto const* ground = app_commands.component<Ground>(entity);
  auto const* scale = app_commands.component<Scale>(entity);
  auto const* transform = app_commands.component<SpawnTransform>(entity);
  auto const* velocity = app_commands.component<Velocity>(entity);
  auto const* acceleration = app_commands.component<Acceleration>(entity);
  auto const* fuel = app_commands.component<Fuel>(entity);
  auto const* rectangle = app_commands.component<Rectangle>(entity);
  auto const* circle = app_commands.component<Circle>(entity);
  auto const* outline = app_commands.component<Outline>(entity);
  auto const* colour = app_commands.component<Colour>(entity);
  auto const* rotation = app_commands.component<Rotation>(entity);
  auto const* z_index = app_commands.component<ZIndex>(entity);
  auto const* affected_by_gravity = app_commands.component<AffectedByGravity>(entity);
  auto const* selectable = app_commands.component<debug::Selectable>(entity);
  auto const* resizeable = app_commands.component<debug::Resizeable>(entity);
  auto const* draggable = app_commands.component<debug::Draggable>(entity);
  auto const* rotatable = app_commands.component<debug::Rotatable>(entity);
  auto const* deletable = app_commands.component<debug::Deletable>(entity);
  auto const* camera_target = app_commands.component<camera::Target>(entity);
  auto const* sticky = app_commands.component<camera::Sticky>(entity);
  auto const* finish_line = app_commands.component<FinishLine>(entity);
  auto const* maximum_velocity = app_commands.component<MaximumVelocity>(entity);

  if (texture != nullptr) { components["texture"] = texture->path; }
  if (image_dimensions != nullptr) { components["image_dimensions"] = serialize_vector(image_dimensions->dimensions); }
  if (ground != nullptr) { components["ground"] = true; }
  if (scale != nullptr) { components["scale"] = serialize_vector(scale->scale); }
  if (transform != nullptr) { components["transform"] = serialize_vector(transform->value); }
  if (velocity != nullptr) { components["velocity"] = serialize_vector(velocity->value); }
  if (acceleration != nullptr) { components["acceleration"] = serialize_vector(acceleration->value); }
  if (fuel != nullptr) { components["fuel"] = serialize_fuel(*fuel); }
  if (rectangle != nullptr) { components["rectangle"] = serialize_vector(rectangle->width_height); }
  if (circle != nullptr) { components["circle"] = circle->radius; }
  if (outline != nullptr) { components["outline"] = serialize_outline(*outline); }
  if (colour != nullptr) { components["colour"] = serialize_colour(*colour); }
  if (rotation != nullptr) { components["rotation"] = rotation->degrees; }
  if (z_index != nullptr) { components["z_index"] = z_index->value; }
  if (affected_by_gravity != nullptr) { components["affected_by_gravity"] = true; }
  if (selectable != nullptr) { components["selectable"] = true; }
  if (resizeable != nullptr) { components["resizeable"] = true; }
  if (draggable != nullptr) { components["draggable"] = true; }
  if (rotatable != nullptr) { components["rotatable"] = true; }
  if (deletable != nullptr) { components["deletable"] = true; }
  if (camera_target != nullptr) { components["camera_target"] = camera_target->is_followed; }
  if (sticky != nullptr) { components["sticky"] = true; }
  if (finish_line != nullptr) { components["finish_line"] = true; }
  if (maximum_velocity != nullptr) { components["maximum_velocity"] = maximum_velocity->value; }

  json object;

  object["components"] = components;
  object["id"] = entity;

  return object;
}

inline Entity deserialize_and_spawn(AppCommands& app_commands, nlohmann::json const& object)
{
  auto components = object["components"];
  auto entity = app_commands.spawn(object["id"]).add_component<Serialized>();

  if (components.contains("texture") && components["texture"].is_string()) {
    entity.add_component<Texture>(components["texture"]);
  }
  if (components.contains("image_dimensions")) {
    entity.add_component<ImageDimensions>(deserialize_vector(components["image_dimensions"]));
  }
  if (components.contains("ground")) { entity.add_component<Ground>(); }
  if (components.contains("scale")) { entity.add_component<Scale>(deserialize_vector(components["scale"])); }
  if (components.contains("transform")) {
    entity.add_component<Transform>(deserialize_vector(components["transform"]));
    entity.add_component<SpawnTransform>(deserialize_vector(components["transform"]));
  }
  if (components.contains("velocity")) { entity.add_component<Velocity>(deserialize_vector(components["velocity"])); }
  if (components.contains("acceleration")) {
    entity.add_component<Acceleration>(deserialize_vector(components["acceleration"]));
  }
  if (components.contains("fuel")) { entity.add_component<Fuel>(deserialize_fuel(components["fuel"])); }
  if (components.contains("rectangle")) {
    entity.add_component<Rectangle>(deserialize_vector(components["rectangle"]));
  }
  if (components.contains("circle")) { entity.add_component<Circle>(components["circle"]); }
  if (components.contains("outline")) { entity.add_component<Outline>(deserialize_outline(components["outline"])); }
  if (components.contains("colour")) { entity.add_component<Colour>(deserialize_colour(components["colour"])); }
  if (components.contains("rotation")) { entity.add_component<Rotation>(components["rotation"]); }
  if (components.contains("z_index")) { entity.add_component<ZIndex>(components["z_index"]); }
  if (components.contains("affected_by_gravity")) { entity.add_component<AffectedByGravity>(); }
  if (components.contains("selectable")) { entity.add_component<debug::Selectable>(); }
  if (components.contains("resizeable")) { entity.add_component<debug::Resizeable>(); }
  if (components.contains("draggable")) { entity.add_component<debug::Draggable>(); }
  if (components.contains("rotatable")) { entity.add_component<debug::Rotatable>(); }
  if (components.contains("deletable")) { entity.add_component<debug::Deletable>(); }
  if (components.contains("camera_target")) { entity.add_component<camera::Target>(components["camera_target"]); }
  if (components.contains("sticky")) { entity.add_component<camera::Sticky>(); }
  if (components.contains("finish_line")) { entity.add_component<FinishLine>(); }
  if (components.contains("maximum_velocity")) {
    entity.add_component<MaximumVelocity>(components["maximum_velocity"]);
  }

  return entity;
}

inline nlohmann::json serialize_outline(Outline const& outline)
{
  using json = nlohmann::json;

  json object;

  object["colour"] = serialize_colour(outline.colour);
  object["thickness"] = outline.thickness;

  return object;
}

inline nlohmann::json serialize_fuel(Fuel const& fuel)
{
  using json = nlohmann::json;

  json object;

  object["burn_rate"] = fuel.burn_rate;
  object["current_fuel"] = fuel.current_fuel;
  object["max_capacity"] = fuel.max_capacity;

  return object;
}


inline nlohmann::json serialize_colour(Colour const& colour)
{
  using json = nlohmann::json;

  json object;

  object["r"] = colour.r;
  object["g"] = colour.g;
  object["b"] = colour.b;
  object["a"] = colour.a;

  return object;
}

inline nlohmann::json serialize_vector(sf::Vector2f const& vector)
{
  using json = nlohmann::json;

  json object;

  object["x"] = vector.x;
  object["y"] = vector.y;

  return object;
}

inline sf::Vector2f deserialize_vector(nlohmann::json const& object)
{
  sf::Vector2f vector;

  vector.x = object["x"];
  vector.y = object["y"];

  return vector;
}
inline Outline deserialize_outline(nlohmann::json const& object)
{
  Outline outline;

  outline.thickness = object["thickness"];
  outline.colour = deserialize_colour(object["colour"]);

  return outline;
}

inline Fuel deserialize_fuel(nlohmann::json const& object)
{
  Fuel fuel;

  fuel.burn_rate = object["burn_rate"];
  fuel.current_fuel = object["current_fuel"];
  fuel.max_capacity = object["max_capacity"];

  return fuel;
}

inline Colour deserialize_colour(nlohmann::json const& object)
{
  Colour colour;

  colour.r = object["r"];
  colour.g = object["g"];
  colour.b = object["b"];
  colour.a = object["a"];

  return colour;
}

}// namespace serialization
