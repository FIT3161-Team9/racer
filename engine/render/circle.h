#pragma once

#include <SFML/Graphics.hpp>

#include "../circle.h"
#include "../colour.h"
#include "../transform.h"

namespace render
{

/// Render a circle
inline void circle(sf::RenderWindow& window, Transform const& transform, Circle const& circle, Colour const& colour)
{
  sf::CircleShape circle_shape(circle.radius);
  // We subtract the radius of the circle because SFML will render the circle relative to the top left,
  // but we want to render it relative to it's center
  circle_shape.setPosition(window::to_screen_space(transform.value) - sf::Vector2f{ circle.radius, circle.radius });
  circle_shape.setFillColor(render_utils::convert_colour(colour));
  window.draw(circle_shape);
}

}// namespace render