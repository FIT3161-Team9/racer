#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>

/// A basic component representing the colour of an entity
struct Colour
{
  /// Red
  std::uint8_t r{};
  /// Green
  std::uint8_t g{};
  /// Blue
  std::uint8_t b{};
  /// Alpha (Transparency)
  std::uint8_t a{ 255 };
};

namespace colour
{

inline Colour alpha(Colour colour, std::uint8_t alpha)
{
  return { .r = colour.r, .g = colour.g, .b = colour.b, .a = alpha };
}

inline Colour red() { return Colour{ .r = 255 }; }

inline Colour orange() { return Colour{ .r = 240, .g = 125, .b = 31 }; }

inline Colour blue() { return Colour{ .b = 255 }; }

inline Colour green() { return Colour{ .g = 255 }; }

inline Colour black() { return Colour{}; }

inline Colour white() { return Colour{ .r = 255, .g = 255, .b = 255 }; }

inline Colour transparent() { return Colour{ .a = 0 }; }

};// namespace colour
