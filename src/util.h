#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

namespace util
{

auto distance_between(sf::Vector2f const& a, sf::Vector2f const& b)
{
  return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
}// namespace util