#pragma once

#include <SFML/Graphics.hpp>

/// A component to represent a triangle in the coordinate system
struct Triangle
{
  sf::Vector2f point_one;
  sf::Vector2f point_two;
  sf::Vector2f point_three;
};
