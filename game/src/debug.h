#pragma once

#include <iostream>
#include <optional>

#include "engine/app_commands.h"
#include "engine/circle.h"
#include "engine/colour.h"
#include "engine/event.h"
#include "engine/outline.h"
#include "engine/query.h"
#include "engine/rectangle.h"
#include "engine/rotation.h"
#include "engine/transform.h"
#include "engine/zindex.h"

#include "entt/entity/fwd.hpp"
#include "game/src/collisions.h"
#include "game/src/ground.h"
#include "game/src/rectangle_utils.h"

namespace debug
{

// Resource for selected entity
struct SelectedEntityResource
{
  std::optional<entt::entity> entity{};
  std::vector<entt::entity> corners{};
};

namespace report_mouse_position
{

  inline void clear_selection(AppCommands& app_commands, SelectedEntityResource& selected_entity_resource)
  {
    if (!selected_entity_resource.entity.has_value()) { return; }
    auto selected_entity_outline = app_commands.component<Outline>(selected_entity_resource.entity.value());
    selected_entity_outline->colour = colour::transparent();

    for (auto corner_entity : selected_entity_resource.corners) { app_commands.destroy(corner_entity); }

    selected_entity_resource.corners.clear();
    selected_entity_resource.entity.reset();
  }

  inline void make_selection(AppCommands& app_commands,
                             SelectedEntityResource& selected_entity_resource,
                             entt::entity entity_to_select,
                             Rectangle const& rectangle,
                             Transform const& transform,
                             Rotation const& rotation,
                             Outline& outline)
  {
    selected_entity_resource.entity = entity_to_select;
    outline.colour = colour::blue();

    auto const corners = rectangle_utils::corners(rectangle, transform, rotation);
    for (auto const& corner : corners) {
      selected_entity_resource.corners.push_back(app_commands.spawn()
                                                   .add_component<Circle>(10.f)
                                                   .add_component<Transform>(corner)
                                                   .template add_component<ZIndex>(4)
                                                   .template add_component<Colour>(colour::blue())
                                                   .entity());
    }
  }

  inline void plugin(AppCommands& app_commands)
  {
    app_commands.add_resource<SelectedEntityResource>();

    app_commands.add_system<Event::EventType::MouseButtonReleased>(
      Query<Transform const, Ground const, Rectangle const, Rotation const, Outline>{}, [&](auto& event, auto& view) {
        auto const selected_entity_resource = app_commands.get_resource<SelectedEntityResource>();
        std::optional<entt::entity> clicked_on_entity{};

        assert(selected_entity_resource);

        for (auto&& [entity, transform, ground, rectangle, rotation, outline] : view.each()) {

          if (!collisions::point_rectangle(rectangle, transform, rotation, event.mouse_button_released.location)) {
            continue;
          }

          clicked_on_entity = entity;

          if (clicked_on_entity != selected_entity_resource->entity) {
            clear_selection(app_commands, *selected_entity_resource);
            make_selection(app_commands, *selected_entity_resource, entity, rectangle, transform, rotation, outline);
          }
          break;
        }

        if (clicked_on_entity != selected_entity_resource->entity) {
          clear_selection(app_commands, *selected_entity_resource);
        }

        return false;
      });
  }

}// namespace report_mouse_position

}// namespace debug
