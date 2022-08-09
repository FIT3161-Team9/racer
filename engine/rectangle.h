#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "transform.h"

/// Component representing a rectangle. A colour and transform will also have
/// to be given in order to render the rectangle (a transform isn't required if
/// it is the child of a layout component)
struct Rectangle
{
  sf::Vector2f width_height;
};
