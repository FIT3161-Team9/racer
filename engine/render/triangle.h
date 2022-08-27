#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "../colour.h"
#include "../render_context.h"
#include "../transform.h"
#include "../triangle.h"

namespace render
{

/// Render a triangle
inline void triangle(RenderContext& render_context,
                     sf::RenderWindow& window,
                     entt::entity entity,
                     Transform const& transform,
                     Triangle const& triangle,
                     Colour const& colour)
{
  (void)render_context;
  (void)entity;
  sf::ConvexShape sf_triangle{};
  sf_triangle.setPointCount(3);
  sf_triangle.setPoint(0, window::to_screen_space(transform.value) - triangle.point_one);
  sf_triangle.setPoint(1, window::to_screen_space(transform.value) - triangle.point_two);
  sf_triangle.setPoint(2, window::to_screen_space(transform.value) - triangle.point_three);
  sf_triangle.setFillColor(render_utils::convert_colour(colour));
  window.draw(sf_triangle);
}

}// namespace render