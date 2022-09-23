#pragma once

#include <vector>

#include <entt/entt.hpp>

#include "engine/app_commands.h"
#include "engine/circle.h"
#include "engine/colour.h"
#include "engine/query.h"
#include "engine/rectangle.h"
#include "engine/rotation.h"
#include "engine/transform.h"
#include "engine/window.h"
#include "engine/zindex.h"

#include "../collisions.h"
#include "../rectangle_utils.h"
#include "../shift_key_resource.h"
#include "./selectable.h"

namespace debug
{

struct Resizeable
{
  bool is_resizing;
};

namespace resizable
{
  struct ResizeCircle
  {
    rectangle_utils::Corner corner;
    entt::entity parent;
  };

  inline void spawn_resize_corner(AppCommands& app_commands,
                                  rectangle_utils::Corner corner,
                                  entt::entity parent,
                                  sf::Vector2f const& transform)
  {
    app_commands.spawn()
      .add_component<ResizeCircle>(corner, parent)
      .template add_component<Circle>(8.f)
      .template add_component<Transform>(transform)
      .template add_component<ZIndex>(4)
      .template add_component<Colour>(colour::blue());
  }

  inline void make_selection(AppCommands& app_commands,
                             entt::entity const selectable_entity,
                             Rectangle const rectangle,
                             Transform const transform,
                             Rotation const rotation)
  {
    auto const corners = rectangle_utils::corners(rectangle, transform, rotation);

    spawn_resize_corner(app_commands, rectangle_utils::Corner::TopRight, selectable_entity, corners[0]);
    spawn_resize_corner(app_commands, rectangle_utils::Corner::TopLeft, selectable_entity, corners[1]);
    spawn_resize_corner(app_commands, rectangle_utils::Corner::BottomRight, selectable_entity, corners[2]);
    spawn_resize_corner(app_commands, rectangle_utils::Corner::BottomLeft, selectable_entity, corners[3]);
  }

  inline void plugin(AppCommands& app_commands)
  {
    // Destroy circles when selectable is first deselected
    app_commands.add_system(Query<ResizeCircle>{}, [&](auto& view) {
      view.each([&](auto const resize_circle_entity, auto const& resize_circle) {
        if (!app_commands.alive(resize_circle.parent)) {
          app_commands.destroy(resize_circle_entity);
          return;
        }
        auto const parent_selectable = app_commands.component<Selectable>(resize_circle.parent);
        if (parent_selectable == nullptr || !parent_selectable->selected) {
          app_commands.destroy(resize_circle_entity);
        }
      });
    });

    // Synchronise "resize circles" with the selected resizeable
    app_commands.add_system(Query<ResizeCircle const, Transform>{}, [&](auto& view) {
      view.each([&](auto const& resize_circle, auto& transform) {
        auto const* parent_transform = app_commands.component<Transform const>(resize_circle.parent);
        if (parent_transform == nullptr) { return; }
        auto const* parent_rectangle = app_commands.component<Rectangle const>(resize_circle.parent);
        if (parent_rectangle == nullptr) { return; }
        auto const* parent_rotation = app_commands.component<Rotation const>(resize_circle.parent);
        if (parent_rotation == nullptr) { return; }

        transform.value =
          rectangle_utils::corner(resize_circle.corner, *parent_rectangle, *parent_transform, *parent_rotation);
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
    app_commands.add_system<Event::EventType::MouseButtonPressed>(
      Query<ResizeCircle const, Circle const, Transform const>{}, [&](auto& event, auto& view) {
        auto const mouse_location = event.mouse_button_pressed.location;
        bool resizing = false;
        view.each([&](auto const& resize_circle, auto const& circle, auto const& transform) {
          if (collisions::point_circle(circle, transform, mouse_location)) {
            app_commands.component<Resizeable>(resize_circle.parent)->is_resizing = true;
            resizing = true;
            return;
          }
        });
        return resizing;
      });

    // Clear `is_resizing` when the mouse is released
    app_commands.add_system<Event::EventType::MouseButtonReleased>(Query<Resizeable>{}, [&](auto& event, auto& view) {
      (void)event;

      view.each([](auto& resizeable) { resizeable.is_resizing = false; });

      return false;
    });
  }

}// namespace resizable


}// namespace debug
