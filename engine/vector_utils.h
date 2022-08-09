#pragma once

#include <cmath>

#include <SFML/Graphics.hpp>

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

}// namespace vector_utils
