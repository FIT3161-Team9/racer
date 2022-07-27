#pragma once

#include <cmath>

#include <SFML/Graphics.hpp>

namespace vector_utils
{

inline float magnitude(sf::Vector2f const& v) { return std::sqrt(v.x * v.x + v.y * v.y); }

inline float dot_product(sf::Vector2f const& a, sf::Vector2f const& b) { return a.x * b.x + a.y * b.y; }

inline sf::Vector2f scale_vector(sf::Vector2f const& v, float s) { return { v.x * s, v.y * s }; }

}// namespace vector_utils
