#pragma once

#include <optional>

#include <SFML/Graphics.hpp>

#include "engine/app_commands.h"
#include "engine/transform.h"
#include "engine/vector_utils.h"

#include "game/src/spawn_transform.h"

namespace debug
{

struct Draggable
{
  // If the entity is being dragged, it will have a drag offset, which is
  // the position of the mouse relative to the center of the entity
  std::optional<sf::Vector2f> drag_offset{};
};

namespace draggable
{
  inline void plugin(AppCommands& app_commands)
  {
    // Update draggable transform when being dragged
    app_commands.add_system<Event::EventType::MouseMoved>(Query<Draggable, Transform>{}, [&](auto& event, auto& view) {
      auto const& mouse_location = event.mouse_moved.location;
      view.each([&](auto entity, auto& draggable, auto& transform) {
        if (!draggable.drag_offset.has_value()) { return; }
        auto const new_transform = vector_utils::minus(mouse_location, draggable.drag_offset.value());
        auto const transform_delta = vector_utils::minus(new_transform, transform.value);
        transform.value = new_transform;
        auto* spawn_transform = app_commands.component<SpawnTransform>(entity);
        if (spawn_transform != nullptr) {
          spawn_transform->value.x += transform_delta.x;
          spawn_transform->value.y += transform_delta.y;
        }
      });
      return false;
    });

    // Clear draggable state on mouse release
    app_commands.add_system<Event::EventType::MouseButtonReleased>(Query<Draggable>{}, [&](auto& event, auto& view) {
      (void)event;
      view.each([&](auto& draggable) { draggable.drag_offset.reset(); });
      return false;
    });
  }
}// namespace draggable

}// namespace debug
