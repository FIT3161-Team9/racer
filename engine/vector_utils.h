#pragma once

#include "rotation.h"
#include <cmath>
#include <string>

#include <SFML/System/Vector2.hpp>

namespace vector_utils
{

/// The magnitude of the given vector
inline float magnitude(sf::Vector2f const& v) { return std::sqrt(v.x * v.x + v.y * v.y); }

/// The dot product between the two given vectors
inline float dot_product(sf::Vector2f const& a, sf::Vector2f const& b) { return a.x * b.x + a.y * b.y; }

/// Return a new vector that is the given vector scaled by the given scalar
inline sf::Vector2f scale_vector(sf::Vector2f const& vector, float scalar)
{
  return { vector.x * scalar, vector.y * scalar };
}

/// Distance between two points
inline float distance_between(sf::Vector2f const& point_one, sf::Vector2f const& point_two)
{
  return std::sqrt((point_one.x - point_two.x) * (point_one.x - point_two.x)
                   + (point_one.y - point_two.y) * (point_one.y - point_two.y));
}

/// Subtract one vector from another
inline sf::Vector2f minus(sf::Vector2f const& vector_one, sf::Vector2f const& vector_two)
{
  return sf::Vector2f{ vector_one.x - vector_two.x, vector_one.y - vector_two.y };
}

inline std::string stringify(sf::Vector2f const& vector)
{
  return "(x=" + std::to_string(vector.x) + ", y=" + std::to_string(vector.y) + ")";
}

inline float angle_between(sf::Vector2f const& a, sf::Vector2f const& b)
{
  auto const angle = std::atan2(a.x * b.y - a.y * b.x, a.x * b.x + a.y * b.y);
  return rotation::from_radians(angle);
}

}// namespace vector_utils
