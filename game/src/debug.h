#pragma once

#include <iostream>
#include <optional>

#include <entt/entt.hpp>

#include "engine/app_commands.h"
#include "engine/circle.h"
#include "engine/colour.h"
#include "engine/event.h"
#include "engine/outline.h"
#include "engine/query.h"
#include "engine/rectangle.h"
#include "engine/rotation.h"
#include "engine/scale.h"
#include "engine/transform.h"
#include "engine/vector_utils.h"
#include "engine/window.h"
#include "engine/zindex.h"

#include "./debug/clipboard.h"
#include "game/src/arrow_keys_resource.h"
#include "game/src/collisions.h"
#include "game/src/debug/deletable.h"
#include "game/src/debug/draggable.h"
#include "game/src/debug/pausable.h"
#include "game/src/debug/resizeable.h"
#include "game/src/debug/rotatable.h"
#include "game/src/debug/selectable.h"
#include "game/src/debug/serialization.h"
#include "game/src/ground.h"
#include "game/src/rectangle_utils.h"
#include "game/src/shift_key_resource.h"

namespace debug
{

namespace drag_and_drop
{
  // When the user presses and holds an arrow key, we don't want to start moving the selected item straight away,
  // so we set a timer and only move after a certain time
  struct MoveOnKeyDownTimer
  {
    std::optional<sf::Clock> timer{};
  };

  inline bool hit_test_entity(AppCommands& app_commands, entt::entity entity, sf::Vector2f mouse_location)
  {
    auto* rectangle = app_commands.component<Rectangle>(entity);
    auto* circle = app_commands.component<Circle>(entity);
    auto* rotation = app_commands.component<Rotation>(entity);
    auto* transform = app_commands.component<Transform>(entity);

    if (rectangle != nullptr && transform != nullptr && rotation != nullptr) {
      return collisions::point_rectangle(*rectangle, *transform, *rotation, mouse_location);
    }

    if (circle != nullptr && transform != nullptr) {
      return collisions::point_circle(*circle, *transform, mouse_location);
    }

    return false;
  }

  inline void plugin(AppCommands& app_commands)
  {
    app_commands.add_resource<MoveOnKeyDownTimer>();
    // Specify whether or not entities are being dragged/selected on mouse down
    app_commands.add_system<Event::EventType::MouseButtonPressed>(
      ResourceQuery<ShiftKeyResource>{},
      Query<Draggable, Selectable, Transform const, Outline>{},
      [&](auto& event, auto& resources, auto& view) {
        auto&& [_, shift_key_resource] = resources;
        bool selected_something = false;
        auto const mouse_location = event.mouse_button_pressed.location;
        for (auto&& [entity, draggable, selectable, transform, outline] : view.each()) {
          // Select entities that pass the hit test
          if (!selected_something && hit_test_entity(app_commands, entity, mouse_location)) {
            selectable.selected = true;
            draggable.drag_offset = vector_utils::minus(mouse_location, transform.value);
            outline.colour = colour::blue();
            resizable::make_selection(app_commands, entity);
            selected_something = true;
            continue;
          }
          // If the shift key is pressed, maintain the last selected entity and update it's drag offset
          if (shift_key_resource.shift_pressed && selectable.selected) {
            draggable.drag_offset = vector_utils::minus(mouse_location, transform.value);
            continue;
          }
          // Clear entities that failed the hit test
          selectable.selected = false;
          draggable.drag_offset.reset();
          outline.colour = colour::transparent();
        }
        return selected_something;
      });

    // Clear draggable state on mouse release
    app_commands.add_system<Event::EventType::MouseButtonReleased>(Query<Draggable>{}, [&](auto& event, auto& view) {
      (void)event;
      view.each([&](auto& draggable) { draggable.drag_offset.reset(); });
      return false;
    });

    // Update draggable transform when being dragged
    app_commands.add_system<Event::EventType::MouseMoved>(Query<Draggable, Transform>{}, [&](auto& event, auto& view) {
      auto const& mouse_location = event.mouse_moved.location;
      view.each([&](auto& draggable, auto& transform) {
        if (!draggable.drag_offset.has_value()) { return; }
        transform.value = vector_utils::minus(mouse_location, draggable.drag_offset.value());
      });
      return false;
    });

    // Move selected item when arrow key is initially pressed, and start the timer
    app_commands.add_system<Event::EventType::KeyPressed>(
      ResourceQuery<MoveOnKeyDownTimer>{},
      Query<Selectable const, Transform>{},
      [&](auto& event, auto& resources, auto& view) {
        auto&& [_, move_on_key_down_timer] = resources;
        if (event.key_pressed.key != sf::Keyboard::Left && event.key_pressed.key != sf::Keyboard::Right
            && event.key_pressed.key != sf::Keyboard::Up && event.key_pressed.key != sf::Keyboard::Down) {
          return false;
        }

        if (!move_on_key_down_timer.timer.has_value()) { move_on_key_down_timer.timer = sf::Clock{}; }
        move_on_key_down_timer.timer.value().restart();

        view.each([&](auto const& selectable, auto& transform) {
          if (!selectable.selected) { return; }
          if (event.key_pressed.key == sf::Keyboard::Left) { transform.value.x -= 1.f; }
          if (event.key_pressed.key == sf::Keyboard::Right) { transform.value.x += 1.f; }
          if (event.key_pressed.key == sf::Keyboard::Down) { transform.value.y += 1.f; }
          if (event.key_pressed.key == sf::Keyboard::Up) { transform.value.y -= 1.f; }
        });
        return false;
      });

    app_commands.add_system<Event::EventType::KeyReleased>(ResourceQuery<MoveOnKeyDownTimer>{},
                                                           [](auto& event, auto& resources) {
                                                             (void)event;
                                                             auto&& [_, move_on_key_down_timer] = resources;
                                                             if (move_on_key_down_timer.timer.has_value()) {
                                                               move_on_key_down_timer.timer.reset();
                                                             }
                                                             return false;
                                                           });

    // Move selected items with arrows keys
    app_commands.add_system(ResourceQuery<ArrowKeysResource, MoveOnKeyDownTimer>{},
                            Query<Selectable const, Transform>{},
                            [&](auto& resources, auto& view) {
                              auto&& [_, arrow_keys_resource, move_on_key_down_timer] = resources;
                              if (!move_on_key_down_timer.timer.has_value()) { return; }
                              auto const time_since_key_was_pressed =
                                move_on_key_down_timer.timer.value().getElapsedTime().asMilliseconds();
                              if (time_since_key_was_pressed < 500) { return; }
                              for (auto&& [entity, selectable, transform] : view.each()) {
                                if (!selectable.selected) { return; }
                                if (arrow_keys_resource.left_pressed) { transform.value.x -= 1.f; }
                                if (arrow_keys_resource.right_pressed) { transform.value.x += 1.f; }
                                if (arrow_keys_resource.up_pressed) { transform.value.y -= 1.f; }
                                if (arrow_keys_resource.down_pressed) { transform.value.y += 1.f; }
                              }
                            });
  }
}// namespace drag_and_drop

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_plugin(pausable::plugin);
  app_commands.add_plugin(rotatable::plugin);
  app_commands.add_plugin(resizable::plugin);
  app_commands.add_plugin(drag_and_drop::plugin);
  app_commands.add_plugin(clipboard::plugin);
  app_commands.add_plugin(deletable::plugin);
  app_commands.add_plugin(serialization::plugin);
}

}// namespace debug
