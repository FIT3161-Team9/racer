#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

namespace util
{

sf::Vector2f const ZERO_VEC = { 0.f, 0.f };

inline auto distance_between(sf::Vector2f const& a, sf::Vector2f const& b)
{
  return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

inline auto magnitude(sf::Vector2f const& vec) { return std::sqrt(vec.x * vec.x + vec.y * vec.y); }

}// namespace util
