#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>

struct Colour
{
  std::uint8_t r{};
  std::uint8_t g{};
  std::uint8_t b{};
  std::uint8_t a{ 255 };
};

namespace colour
{

Colour alpha(Colour colour, std::uint8_t alpha) { return { .r = colour.r, .g = colour.g, .b = colour.b, .a = alpha }; }

Colour red() { return Colour{ .r = 255 }; }

Colour blue() { return Colour{ .b = 255 }; }

Colour green() { return Colour{ .r = 255 }; }

Colour black() { return Colour{}; }

Colour white() { return Colour{ .r = 255, .g = 255, .b = 255 }; }

};// namespace colour
