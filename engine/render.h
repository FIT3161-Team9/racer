#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "circle.h"
#include "colour.h"
#include "transform.h"
#include "window.h"

namespace render
{
void circle(sf::RenderWindow&, Transform const&, Circle const&, Colour const&);

void all(sf::RenderWindow& window, entt::registry& registry)
{
  auto view = registry.view<Transform const, Circle const, Colour const>();
  view.each([&](auto const& transform, auto const& circle, auto const& colour) {
    render::circle(window, transform, circle, colour);
  });
}

void circle(sf::RenderWindow& window, Transform const& transform, Circle const& circle, Colour const& colour)
{
  sf::CircleShape circle_shape(circle.radius);
  // We subtract the radius of the circle because SFML will render the circle relative to the top left,
  // but we want to render it relative to it's center
  circle_shape.setPosition(window::to_screen_space(transform.value) - sf::Vector2f{ circle.radius, circle.radius });
  circle_shape.setFillColor(sf::Color(colour.r, colour.g, colour.b, colour.a));
  window.draw(circle_shape);
}

};// namespace render