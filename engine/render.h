#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <entt/entt.hpp>

#include "SFML/System/Vector2.hpp"
#include "children.h"
#include "circle.h"
#include "colour.h"
#include "layout.h"
#include "outline.h"
#include "rectangle.h"
#include "render_context.h"
#include "render_utils.h"
#include "rotation.h"
#include "scale.h"
#include "text.h"
#include "texture.h"
#include "transform.h"
#include "triangle.h"
#include "vector_utils.h"
#include "window.h"
#include "zindex.h"

#include "render/circle.h"
#include "render/rectangle.h"
#include "render/text.h"
#include "render/texture.h"
#include "render/triangle.h"

namespace render
{

inline void root_flex_box(RenderContext& render_context,
                          sf::RenderWindow& window,
                          entt::entity flex_parent,
                          layout::Flex const& layout);

/// Render all entities in the ECS that meet the criteria to be rendered. See the parameters of each
/// of the individual render methods to see what the criteria is to be rendered
inline void all(RenderContext& render_context, sf::RenderWindow& window, entt::registry& registry)
{
  // Everything that wants to be rendered outside of a flexbox needs to specify at least a zindex and a transform
  auto z_index_group = registry.group<>(entt::get<ZIndex const, Transform const>);
  z_index_group.sort<ZIndex const>([](const auto& lhs, const auto& rhs) { return lhs.value < rhs.value; });
  z_index_group.each([&](auto const& entity, auto const& z_index, auto const& transform) {
    (void)z_index;

    auto const* texture = render_context.get_component<Texture const>(entity);
    auto const* scale = render_context.get_component<Scale const>(entity);
    if (texture != nullptr && scale != nullptr) {
      render::texture(render_context, window, transform, *texture, *scale);
    }

    auto const* rectangle = render_context.get_component<Rectangle const>(entity);
    auto const* colour = render_context.get_component<Colour const>(entity);
    if (rectangle != nullptr && colour != nullptr) {
      render::rectangle(render_context, window, entity, transform, *rectangle, *colour);
    }

    auto const* triangle = render_context.get_component<Triangle const>(entity);
    if (triangle != nullptr && colour != nullptr) {
      render::triangle(render_context, window, entity, transform, *triangle, *colour);
    }

    auto const* circle = render_context.get_component<Circle const>(entity);
    if (circle != nullptr && colour != nullptr) { render::circle(window, transform, *circle, *colour); }

    auto const* text = render_context.get_component<Text const>(entity);
    if (text != nullptr && colour != nullptr) { render::text(render_context, window, transform, *text, *colour); }
  });

  auto flex_boxes = registry.view<layout::FlexRoot, layout::Flex, Children>();
  for (auto [flex_parent, _flex_root, flex, _children] : flex_boxes.each()) {
    render::root_flex_box(render_context, window, flex_parent, flex);
  }
}

inline void root_flex_box(RenderContext& render_context,
                          sf::RenderWindow& window,
                          entt::entity flex_parent,
                          layout::Flex const& layout)
{
  auto const root_flex_box =
    layout::FlexContext{ .height = window::COORDINATE_SPACE_HEIGHT, .width = window::COORDINATE_SPACE_WIDTH };
  layout::layout_flex(render_context, window, root_flex_box, layout, flex_parent);
}

};// namespace render
