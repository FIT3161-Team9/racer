#pragma once

#include <optional>

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
#include "game/src/debug/selectable.h"

namespace debug
{
struct Rotateable
{
  std::optional<entt::entity> rotate_handle{};
  // Whether the user is rotating the entity - i.e. whether the user has clicked and held on the rotate handle
  bool rotating{ false };
};

namespace rotateable
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
    app_commands.add_system(
      Query<debug::Selectable, Rotateable, Rectangle const, Transform const, Rotation const>{}, [&](auto& view) {
        view.each(
          [&](auto& selectable, auto& rotateable, auto const& rectangle, auto const& transform, auto const& rotation) {
            if (!selectable.selected || rotateable.rotate_handle.has_value()) { return; }

            auto handle_position = rotate_handle_position(rectangle, transform, rotation);

            rotateable.rotate_handle = app_commands.spawn()
                                         .add_component<Transform>(handle_position)
                                         .template add_component<Circle>(8.f)
                                         .template add_component<ZIndex>(5)
                                         .template add_component<Colour>(colour::red())
                                         .entity();
          });
      });
    // Destroy rotate handle on deselect
    app_commands.add_system(Query<Selectable, Rotateable>{}, [&](auto& view) {
      view.each([&](auto& selectable, auto& rotateable) {
        if (selectable.selected || !rotateable.rotate_handle.has_value()) { return; }

        app_commands.destroy(*rotateable.rotate_handle);
        rotateable.rotate_handle.reset();
      });
    });
    // Keep the position of the rotate handle up to date
    app_commands.add_system(
      Query<Rotateable const, Rectangle const, Transform const, Rotation const>{}, [&](auto& view) {
        view.each([&](auto const& rotateable, auto const& rectangle, auto const& transform, auto const& rotation) {
          if (!rotateable.rotate_handle.has_value()) { return; }
          app_commands.component<Transform>(rotateable.rotate_handle.value())->value =
            rotate_handle_position(rectangle, transform, rotation);
        });
      });
    // TODO: Hit test the rotate_handle
    // TODO: Rotate the entity when the rotate_handle.rotating is true
  }

}// namespace rotateable
}// namespace debug
