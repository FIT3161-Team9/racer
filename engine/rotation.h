#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <numbers>

struct Rotation
{
  float degrees;
};

namespace rotation
{

inline sf::Vector2f
  rotate_about_point(sf::Vector2f const& to_rotate, sf::Vector2f const& point_to_rotate_about, float degrees)
{
  auto const radians = (degrees / 180.f * std::numbers::pi_v<float>);
  float s = std::sin(radians);
  float c = std::cos(radians);

  return {
    c * (to_rotate.x - point_to_rotate_about.x) - s * (to_rotate.y - point_to_rotate_about.y) + point_to_rotate_about.x,
    s * (to_rotate.x - point_to_rotate_about.x) + c * (to_rotate.y - point_to_rotate_about.y) + point_to_rotate_about.y
  };
}

inline float to_radians(float degrees) { return degrees / 180.f * std::numbers::pi_v<float>; }

}// namespace rotation
