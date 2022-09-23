#pragma once

#include <iostream>
#include <optional>

#include "SFML/Window/Keyboard.hpp"
#include <entt/entt.hpp>

#include "engine/app_commands.h"
#include "engine/colour.h"
#include "engine/entity.h"
#include "engine/event.h"
#include "engine/outline.h"
#include "engine/query.h"
#include "engine/rectangle.h"
#include "engine/rotation.h"
#include "engine/scale.h"
#include "engine/texture.h"
#include "engine/transform.h"
#include "engine/window.h"
#include "engine/zindex.h"

#include "game/src/debug/deletable.h"
#include "game/src/debug/draggable.h"
#include "game/src/debug/resizeable.h"
#include "game/src/debug/rotateable.h"
#include "game/src/debug/selectable.h"
#include "game/src/ground.h"
#include "game/src/image_dimensions.h"

struct Clipboard
{
  std::optional<entt::entity> entity{};
};

namespace clipboard
{

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_resource<Clipboard>();

  app_commands.add_system<Event::EventType::KeyReleased>(
    ResourceQuery<Clipboard>{}, Query<debug::Selectable>{}, [&](auto& event, auto& resources, auto& view) {
      if (event.key_released.key != sf::Keyboard::C || !event.key_released.control) { return false; }

      auto&& [_, clipboard] = resources;

      for (auto&& [entity, selectable] : view.each()) {
        if (!selectable.selected) { continue; }

        clipboard.entity = entity;
        break;
      }

      return false;
    });

  app_commands.add_system<Event::EventType::KeyReleased>(ResourceQuery<Clipboard>{}, [&](auto& event, auto& resources) {
    if (event.key_released.key != sf::Keyboard::V || !event.key_released.control) { return false; }

    auto&& [_, clipboard] = resources;

    if (!clipboard.entity.has_value()) { return false; }

    auto const* texture = app_commands.component<Texture>(*clipboard.entity);
    auto const* image_dimensions = app_commands.component<ImageDimensions>(*clipboard.entity);
    auto const* ground = app_commands.component<Ground>(*clipboard.entity);
    auto const* scale = app_commands.component<Scale>(*clipboard.entity);
    auto const* transform = app_commands.component<Transform>(*clipboard.entity);
    auto const* rectangle = app_commands.component<Rectangle>(*clipboard.entity);
    auto const* outline = app_commands.component<Outline>(*clipboard.entity);
    auto const* colour = app_commands.component<Colour>(*clipboard.entity);
    auto const* rotation = app_commands.component<Rotation>(*clipboard.entity);
    auto const* z_index = app_commands.component<ZIndex>(*clipboard.entity);

    auto copied = app_commands.spawn();

    if (texture) { copied.add_component<Texture>(*texture); }
    if (image_dimensions) { copied.add_component<ImageDimensions>(*image_dimensions); }
    if (ground) { copied.add_component<Ground>(*ground); }
    if (scale) { copied.add_component<Scale>(*scale); }
    if (transform) { copied.add_component<Transform>(*transform); }
    if (rectangle) { copied.add_component<Rectangle>(*rectangle); }
    if (outline) { copied.add_component<Outline>(*outline); }
    if (colour) { copied.add_component<Colour>(*colour); }
    if (rotation) { copied.add_component<Rotation>(*rotation); }
    if (z_index) { copied.add_component<ZIndex>(*z_index); }

    copied.add_component<debug::Rotateable>()
      .add_component<debug::Selectable>()
      .add_component<debug::Resizeable>()
      .add_component<debug::Draggable>()
      .add_component<debug::Deletable>();

    return false;
  });
}

}// namespace clipboard
