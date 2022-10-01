#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "../circle.h"
#include "../colour.h"
#include "../outline.h"
#include "../render_context.h"
#include "../render_utils.h"
#include "../transform.h"
#include "../window.h"

namespace render
{

/// Render a circle
inline void circle(RenderContext& render_context,
                   sf::RenderWindow& window,
                   entt::entity entity,
                   Transform const& transform,
                   Circle const& circle,
                   Colour const& colour)
{
  sf::CircleShape circle_shape(circle.radius);
  // We subtract the radius of the circle because SFML will render the circle relative to the top left,
  // but we want to render it relative to it's center
  circle_shape.setPosition(window::to_screen_space(transform.value) - sf::Vector2f{ circle.radius, circle.radius });
  circle_shape.setFillColor(render_utils::convert_colour(colour));

  auto* outline = render_context.get_component<Outline>(entity);
  if (outline != nullptr) {
    circle_shape.setOutlineColor(render_utils::convert_colour(outline->colour));
    circle_shape.setOutlineThickness(outline->thickness);
  }

  window.draw(circle_shape);
}

}// namespace render
