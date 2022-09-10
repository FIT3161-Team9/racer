#pragma once

#include <SFML/Graphics.hpp>

/// A way to scale a texture. This scales the texture up by the values given in each dimension.
///
/// For example Scale{ sf::Vector2f { 2.f, 1.f } } would make a texture twice as wide as it's native
/// resolution
struct Scale
{
  sf::Vector2f scale;
};
