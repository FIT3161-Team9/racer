#pragma once

#include <SFML/Graphics.hpp>

/// Represents a location within the coordinates system (the origin of the
/// coordinate system is the middle of the screen, y positive is down, x positive
/// is to the right of screen)
struct Transform
{
  sf::Vector2f value;
};
