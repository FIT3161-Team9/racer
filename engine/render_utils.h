#pragma once

#include <SFML/Graphics.hpp>

#include "colour.h"

namespace render_utils
{

inline sf::Color convert_colour(Colour const& colour) { return { colour.r, colour.g, colour.b, colour.a }; }

};// namespace render_utils
