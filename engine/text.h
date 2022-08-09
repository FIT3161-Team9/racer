#pragma once

#include <string>

#include "SFML/System/Vector2.hpp"
#include "colour.h"

/// Component for storing everything required to render text to the screen
struct Text
{
  std::string font;
  std::string content;
  uint32_t character_size;
  float letter_spacing{};
};
