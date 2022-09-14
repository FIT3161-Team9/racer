#pragma once

#include <optional>

#include <SFML/Graphics.hpp>

namespace debug
{

struct Draggable
{
  // If the entity is being dragged, it will have a drag offset, which is
  // the position of the mouse relative to the center of the entity
  std::optional<sf::Vector2f> drag_offset{};
};

}// namespace debug
