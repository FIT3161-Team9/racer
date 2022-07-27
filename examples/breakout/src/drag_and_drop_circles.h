#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

#include <engine/app_commands.h>
#include <engine/circle.h>
#include <engine/engine.h>
#include <engine/event.h>
#include <engine/transform.h>

#include "gravity.h"
#include "util.h"
#include "velocity.h"

namespace drag_and_drop_circles
{

struct Draggable
{
  std::optional<sf::Vector2f> grab_offset{};
};

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system<Event::EventType::MouseButtonPressed,
                          Transform const,
                          Velocity,
                          Circle const,
                          Draggable,
                          gravity::AffectedByGravity>([&](auto event, auto& view) {
    for (auto [entity, transform, velocity, circle, draggable, affected_by_gravity] : view.each()) {
      if (util::distance_between(event.mouse_button_pressed.location, transform.value) < circle.radius) {
        velocity.value = { 0.f, 0.f };
        draggable.grab_offset = { event.mouse_button_pressed.location.x - transform.value.x,
                                  event.mouse_button_pressed.location.y - transform.value.y };
        affected_by_gravity.value = false;
        return;
      }
    }
  });

  app_commands.add_system<Event::EventType::MouseButtonReleased, Draggable, gravity::AffectedByGravity>(
    [&](auto, auto& view) {
      for (auto [entity, draggable, affected_by_gravity] : view.each()) {
        draggable.grab_offset = std::nullopt;
        affected_by_gravity.value = true;
      }
    });

  app_commands.add_system<Event::EventType::MouseMoved, Draggable const, Velocity, Transform>(
    [&](auto event, auto& view) {
      for (auto [entity, draggable, velocity, transform] : view.each()) {
        if (draggable.grab_offset.has_value()) {
          transform.value.x = event.mouse_moved.location.x - draggable.grab_offset->x;
          transform.value.y = event.mouse_moved.location.y - draggable.grab_offset->y;
        }
      }
    });
};

}// namespace drag_and_drop_circles
