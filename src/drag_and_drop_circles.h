#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

#include "../engine/app_commands.h"
#include "../engine/circle.h"
#include "../engine/engine.h"
#include "../engine/event.h"
#include "../engine/transform.h"

#include "app_state.h"
#include "util.h"

namespace drag_and_drop_circles
{

struct Draggable
{
  sf::Vector2f grab_offset{};
};

void plugin(AppCommands<AppState>& app_commands)
{
  app_commands.add_system<Event::EventType::MouseButtonPressed, Transform const, Circle const, Draggable>(
    [&](auto event, auto const& view) {
      for (auto [entity, transform, circle, draggable] : view.each()) {
        if (util::distance_between(event.mouse_button_pressed.location, transform.value) < circle.radius) {
          app_commands.set_state(entity);
          draggable.grab_offset.x = event.mouse_button_pressed.location.x - transform.value.x;
          draggable.grab_offset.y = event.mouse_button_pressed.location.y - transform.value.y;
          return;
        }
      }
    });

  app_commands.add_system<Event::EventType::MouseButtonReleased>([&](auto) { app_commands.set_state(std::nullopt); });

  app_commands.add_system<Event::EventType::MouseMoved>([&](auto event) {
    if (app_commands.current_state()) {
      auto& transform = *app_commands.component<Transform>(*app_commands.current_state());
      auto& draggable = *app_commands.component<Draggable>(*app_commands.current_state());
      transform.value.x = event.mouse_moved.location.x - draggable.grab_offset.x;
      transform.value.y = event.mouse_moved.location.y - draggable.grab_offset.y;
    }
  });
};

}// namespace drag_and_drop_circles