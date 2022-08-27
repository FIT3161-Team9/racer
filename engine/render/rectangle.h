#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "../colour.h"
#include "../rectangle.h"
#include "../render_context.h"
#include "../transform.h"

namespace render
{

/// Render a rectangle
inline void rectangle(RenderContext& render_context,
                      sf::RenderWindow& window,
                      entt::entity entity,
                      Transform const& transform,
                      Rectangle const& rectangle,
                      Colour const& colour)
{
  sf::RectangleShape rectangle_shape{};
  rectangle_shape.setOrigin(sf::Vector2f{ 0.5f * rectangle.width_height.x, 0.5f * rectangle.width_height.y });
  rectangle_shape.setPosition(window::to_screen_space(transform.value));
  rectangle_shape.setSize(rectangle.width_height);
  rectangle_shape.setFillColor(sf::Color(colour.r, colour.g, colour.b, colour.a));
  auto* outline = render_context.get_component<Outline>(entity);
  if (outline) {
    auto outline_colour = outline->colour;
    rectangle_shape.setOutlineColor(sf::Color(outline_colour.r, outline_colour.g, outline_colour.b, outline_colour.a));
    rectangle_shape.setOutlineThickness(outline->thickness);
  }
  auto* rotation_component = render_context.get_component<Rotation>(entity);
  auto rotation = rotation_component ? rotation_component->degrees : 0.f;
  if (rotation != 0.f) { rectangle_shape.rotate(rotation); }
  window.draw(rectangle_shape);
};


}// namespace render
