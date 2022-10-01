#pragma once

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "SFML/Window/Keyboard.hpp"
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "engine/app_commands.h"
#include "engine/colour.h"
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

namespace serialization
{

nlohmann::json serialize(AppCommands& app_commands, entt::entity entity);
nlohmann::json serialize_outline(Outline const&);
nlohmann::json serialize_colour(Colour const&);
nlohmann::json serialize_vector(sf::Vector2f const&);
sf::Vector2f deserialize_vector(nlohmann::json const&);
Outline deserialize_outline(nlohmann::json const&);
Colour deserialize_colour(nlohmann::json const&);

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system<Event::EventType::KeyReleased>(Query<Transform>{}, [&](auto& event, auto& view) {
    if (event.key_released.key != sf::Keyboard::D) { return false; }

    auto time = std::time(nullptr);
    char file_name[100];
    std::strftime(file_name, sizeof(file_name), "%Y-%m-%d_%I.%M.%S.json", std::localtime(&time));

    std::ofstream outfile(file_name);

    nlohmann::json array;

    for (auto&& [entity, _transform] : view.each()) { array.push_back(serialize(app_commands, entity)); }

    outfile << array.dump();

    outfile.close();

    return true;
  });
}

inline nlohmann::json serialize(AppCommands& app_commands, entt::entity entity)
{
  using json = nlohmann::json;

  json object;

  auto const* texture = app_commands.component<Texture>(entity);
  auto const* image_dimensions = app_commands.component<ImageDimensions>(entity);
  auto const* ground = app_commands.component<Ground>(entity);
  auto const* scale = app_commands.component<Scale>(entity);
  auto const* transform = app_commands.component<Transform>(entity);
  auto const* rectangle = app_commands.component<Rectangle>(entity);
  auto const* outline = app_commands.component<Outline>(entity);
  auto const* colour = app_commands.component<Colour>(entity);
  auto const* rotation = app_commands.component<Rotation>(entity);
  auto const* z_index = app_commands.component<ZIndex>(entity);

  if (texture != nullptr) { object["texture"] = texture->path; }
  if (image_dimensions != nullptr) { object["image_dimensions"] = serialize_vector(image_dimensions->dimensions); }
  if (ground != nullptr) { object["ground"] = true; }
  if (scale != nullptr) { object["scale"] = serialize_vector(scale->scale); }
  if (transform != nullptr) { object["transform"] = serialize_vector(transform->value); }
  if (rectangle != nullptr) { object["rectangle"] = serialize_vector(rectangle->width_height); }
  if (outline != nullptr) { object["outline"] = serialize_outline(*outline); }
  if (colour != nullptr) { object["colour"] = serialize_colour(*colour); }
  if (rotation != nullptr) { object["rotation"] = rotation->degrees; }
  if (z_index != nullptr) { object["z_index"] = z_index->value; }

  return object;
}

inline void deserialize_and_spawn(AppCommands& app_commands, nlohmann::json const& object)
{
  auto entity = app_commands.spawn();

  if (object["texture"].is_string()) { entity.add_component<Texture>(object["texture"]); }
  if (object["image_dimensions"].is_object()) {
    entity.add_component<ImageDimensions>(deserialize_vector(object["image_dimensions"]));
  }
  if (object["ground"]) { entity.add_component<Ground>(); }
  if (object["scale"].is_object()) { entity.add_component<Scale>(deserialize_vector(object["scale"])); }
  if (object["tranform"].is_object()) { entity.add_component<Transform>(deserialize_vector(object["transform"])); }
  if (object["rectangle"].is_object()) { entity.add_component<Rectangle>(deserialize_vector(object["rectangle"])); }
  if (object["outline"].is_object()) { entity.add_component<Outline>(deserialize_outline(object["outline"])); }
  if (object["colour"].is_object()) { entity.add_component<Colour>(deserialize_colour(object["colour"])); }
  if (object["rotation"].is_number_float()) { entity.add_component<Rotation>(object["rotation"]); }
  if (object["z_index"].is_number_float()) { entity.add_component<ZIndex>(object["z_index"]); }
}

inline nlohmann::json serialize_outline(Outline const& outline)
{
  using json = nlohmann::json;

  json object;

  object["colour"] = serialize_colour(outline.colour);
  object["thickness"] = outline.thickness;

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
