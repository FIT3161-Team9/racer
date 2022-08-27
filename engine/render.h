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
#include "text.h"
#include "texture.h"
#include "transform.h"
#include "triangle.h"
#include "vector_utils.h"
#include "window.h"

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
  // TODO : Render according to a Z Order?
  auto texture_view = registry.view<Transform const, Texture const>();
  texture_view.each(
    [&](auto const& transform, auto const& texture) { render::texture(render_context, window, transform, texture); });
  auto rectangle_view = registry.view<Transform const, Rectangle const, Colour const>();
  for (auto [entity, transform, rectangle, colour] : rectangle_view.each()) {
    render::rectangle(render_context, window, entity, transform, rectangle, colour);
  }
  auto triangle_view = registry.view<Transform const, Colour const, Triangle const>();
  for (auto [entity, transform, colour, triangle] : triangle_view.each()) {
    render::triangle(render_context, window, entity, transform, triangle, colour);
  }
  auto circle_view = registry.view<Transform const, Circle const, Colour const>();
  circle_view.each([&](auto const& transform, auto const& circle, auto const& colour) {
    render::circle(window, transform, circle, colour);
  });
  auto text_view = registry.view<Transform const, Text const, Colour const>();
  for (auto [_entity, transform, text, colour] : text_view.each()) {
    render::text(render_context, window, transform, text, colour);
  }
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
