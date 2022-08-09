#pragma once

#include <SFML/Graphics.hpp>

#include "colour.h"

namespace render_utils
{

/// Given an engine Colour component, return a corresponding SFML Color
inline sf::Color convert_colour(Colour const& colour) { return { colour.r, colour.g, colour.b, colour.a }; }

};// namespace render_utils
