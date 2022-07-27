#pragma once

#include <string>

#include "SFML/System/Vector2.hpp"
#include "colour.h"

struct Text
{
  std::string font;
  std::string content;
  uint32_t character_size;
};
