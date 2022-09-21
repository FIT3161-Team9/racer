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
#include "game/src/debug/draggable.h"
#include "game/src/debug/resizeable.h"
#include "game/src/debug/rotateable.h"
#include "game/src/debug/selectable.h"
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

  inline void plugin(AppCommands& app_commands)
  {
    app_commands.add_resource<MoveOnKeyDownTimer>();
    // Specify whether or not entities are being dragged/selected on mouse down
    app_commands.add_system<Event::EventType::MouseButtonPressed>(
      Query<Draggable, Selectable, Transform const, Rectangle const, Rotation const, Outline>{},
      [&](auto& event, auto& view) {
        bool selected_something = false;
        auto const mouse_location = event.mouse_button_pressed.location;
        for (auto&& [entity, draggable, selectable, transform, rectangle, rotation, outline] : view.each()) {
          if (!selected_something && collisions::point_rectangle(rectangle, transform, rotation, mouse_location)) {
            selectable.selected = true;
            draggable.drag_offset = vector_utils::minus(mouse_location, transform.value);
            outline.colour = colour::blue();
            selected_something = true;
            continue;
          }
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

namespace resize
{
  inline void plugin(AppCommands& app_commands)
  {
    // Spawn circles when selectable is first selected
    app_commands.add_system(
      Query<Selectable, Resizeable, Rectangle const, Transform const, Rotation const>{}, [&](auto& view) {
        view.each(
          [&](auto& selectable, auto& resizeable, auto const& rectangle, auto const& transform, auto const& rotation) {
            if (!selectable.selected || resizeable.resize_circles.size() != 0) { return; }
            auto const corners = rectangle_utils::corners(rectangle, transform, rotation);
            for (auto const& corner : corners) {
              resizeable.resize_circles.push_back(app_commands.spawn()
                                                    .add_component<Circle>(8.f)
                                                    .add_component<Transform>(corner)
                                                    .template add_component<ZIndex>(4)
                                                    .template add_component<Colour>(colour::blue())
                                                    .entity());
            }
          });
      });

    // Destroy circles when selectable is first deselected
    app_commands.add_system(Query<Selectable, Resizeable>{}, [&](auto& view) {
      view.each([&](auto& selectable, auto& resizeable) {
        if (selectable.selected || resizeable.resize_circles.size() == 0) { return; }

        for (auto const& corner : resizeable.resize_circles) { app_commands.destroy(corner); }
        resizeable.resize_circles.clear();
      });
    });

    // Synchronise "resize circles" with the selected resizeable
    app_commands.add_system(
      Query<Selectable const, Resizeable const, Rectangle const, Transform const, Rotation const>{}, [&](auto& view) {
        view.each([&](auto const& selectable,
                      auto const& resizeable,
                      auto const& rectangle,
                      auto const& transform,
                      auto const& rotation) {
          if (!selectable.selected) { return; }
          auto const corners = rectangle_utils::corners(rectangle, transform, rotation);
          for (auto i = 0; i < 4; i++) {
            auto& resize_circle_transform = *app_commands.component<Transform>(resizeable.resize_circles[i]);
            resize_circle_transform.value = corners[i];
          }
        });
      });

    // Resize selected objects when mouse moves and is pressed
    app_commands.add_system<Event::EventType::MouseMoved>(
      ResourceQuery<ShiftKeyResource>{},
      Query<Resizeable, Transform const, Rectangle>{},
      [&](auto& event, auto& resources, auto& view) {
        auto const& mouse_location = event.mouse_moved.location;
        auto&& [_, shift_key_resource] = resources;
        for (auto&& [_entity, resizeable, transform, rectangle] : view.each()) {

          if (!resizeable.is_resizing) { continue; }
          auto const width = std::abs((mouse_location.x - transform.value.x) * 2.f);
          auto const height = std::abs((mouse_location.y - transform.value.y) * 2.f);

          if (!shift_key_resource.shift_pressed) {
            rectangle.width_height.x = width;
            rectangle.width_height.y = height;
            continue;
          }

          auto [target_width_scale, target_height_scale] = window::scale_required_to_satisfy_aspect_ratio(
            { width, height }, rectangle.width_height.x / rectangle.width_height.y);

          rectangle.width_height.x = width * target_width_scale;
          rectangle.width_height.y = height * target_height_scale;
        }

        return false;
      });

    // Hittest the resize circles
    app_commands.add_system<Event::EventType::MouseButtonPressed>(Query<Resizeable>{}, [&](auto& event, auto& view) {
      auto const mouse_location = event.mouse_button_pressed.location;
      for (auto&& [_entity, resizeable] : view.each()) {
        for (auto& circle_entity : resizeable.resize_circles) {
          auto& corner_circle = *app_commands.component<Circle>(circle_entity);
          auto& corner_transform = *app_commands.component<Transform>(circle_entity);
          if (collisions::point_circle(corner_circle, corner_transform, mouse_location)) {
            resizeable.is_resizing = true;
            return true;
          }
        }
      }
      return false;
    });

    // Clear `is_resizing` when the mouse is released
    app_commands.add_system<Event::EventType::MouseButtonReleased>(Query<Resizeable>{}, [&](auto& event, auto& view) {
      (void)event;

      view.each([](auto& resizeable) { resizeable.is_resizing = false; });

      return false;
    });
  }

}// namespace resize

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_plugin(resize::plugin);
  app_commands.add_plugin(drag_and_drop::plugin);
  app_commands.add_plugin(clipboard::plugin);
  app_commands.add_plugin(rotateable::plugin);
}

}// namespace debug
