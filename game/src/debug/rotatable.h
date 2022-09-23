#pragma once

#include <optional>

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/System/Vector2.hpp"
#include <entt/entt.hpp>

#include "engine/app_commands.h"
#include "engine/circle.h"
#include "engine/colour.h"
#include "engine/query.h"
#include "engine/rectangle.h"
#include "engine/rotation.h"
#include "engine/transform.h"
#include "engine/zindex.h"

#include "engine/zindex.h"
#include "game/src/collisions.h"
#include "game/src/debug/selectable.h"

namespace debug
{
struct Rotatable
{
  std::optional<entt::entity> rotate_handle{};
  // Whether the user is rotating the entity - i.e. whether the user has clicked and held on the rotate handle
  bool rotating{ false };
};

struct RotateHandle
{
  entt::entity parent;
};

namespace rotatable
{

  inline sf::Vector2f
    rotate_handle_position(Rectangle const& rectangle, Transform const& transform, Rotation const& rotation)
  {
    return rotation::rotate_about_point(sf::Vector2f{ transform.value.x + rectangle.width_height.x * 0.5f + 20.f,
                                                      transform.value.y - rectangle.width_height.y * 0.5f - 20.f },
                                        transform.value,
                                        rotation.degrees);
  }

  inline void plugin(AppCommands& app_commands)
  {
    // Spawn rotate handle on select
    app_commands.add_system(Query<debug::Selectable, Rotatable, Rectangle const, Transform const, Rotation const>{},
                            [&](auto& view) {
                              view.each([&](auto selectable_entity,
                                            auto& selectable,
                                            auto& rotatable,
                                            auto const& rectangle,
                                            auto const& transform,
                                            auto const& rotation) {
                                if (!selectable.selected || rotatable.rotate_handle.has_value()) { return; }

                                auto handle_position = rotate_handle_position(rectangle, transform, rotation);

                                rotatable.rotate_handle = app_commands.spawn()
                                                            .add_component<Transform>(handle_position)
                                                            .template add_component<RotateHandle>(selectable_entity)
                                                            .template add_component<Circle>(8.f)
                                                            .template add_component<ZIndex>(5)
                                                            .template add_component<Colour>(colour::red())
                                                            .entity();
                              });
                            });
    // Destroy rotate handle on parent deletion
    app_commands.add_system(Query<RotateHandle>{}, [&](auto& view) {
      view.each([&](auto const rotate_handle_entity, auto const& rotate_handle) {
        if (!app_commands.alive(rotate_handle.parent)) { app_commands.destroy(rotate_handle_entity); }
      });
    });
    // Destroy rotate handle on deselect or parent deletion
    app_commands.add_system(Query<Selectable, Rotatable>{}, [&](auto& view) {
      view.each([&](auto& selectable, auto& rotatable) {
        if (selectable.selected || !rotatable.rotate_handle.has_value()) { return; }

        app_commands.destroy(*rotatable.rotate_handle);
        rotatable.rotate_handle.reset();
      });
    });
    // Keep the position of the rotate handle up to date
    app_commands.add_system(
      Query<Rotatable const, Rectangle const, Transform const, Rotation const>{}, [&](auto& view) {
        view.each([&](auto const& rotatable, auto const& rectangle, auto const& transform, auto const& rotation) {
          if (!rotatable.rotate_handle.has_value()) { return; }
          app_commands.component<Transform>(rotatable.rotate_handle.value())->value =
            rotate_handle_position(rectangle, transform, rotation);
        });
      });
    // Hit test the rotate_handle
    app_commands.add_system<Event::EventType::MouseButtonPressed>(
      Query<RotateHandle, Transform, Circle>{}, [&](auto& event, auto& view) {
        auto const mouse_location = event.mouse_button_pressed.location;
        for (auto&& [_entity, rotate_handle, transform, circle] : view.each()) {
          if (collisions::point_circle(circle, transform, mouse_location)) {
            app_commands.component<Rotatable>(rotate_handle.parent)->rotating = true;
            return true;
          }
        };
        return false;
      });
    // Rotate the entity when the rotate_handle.rotating is true
    app_commands.add_system<Event::EventType::MouseMoved>(Query<Rotatable const, Transform const, Rotation>{},
                                                          [&](auto& event, auto& view) {
                                                            (void)event;
                                                            (void)view;
                                                            return false;
                                                          });
  }

}// namespace rotatable
}// namespace debug
