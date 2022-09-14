#pragma once

#include <SFML/Graphics.hpp>

#include "engine/app_commands.h"
#include "engine/query.h"
#include "engine/rectangle.h"
#include "engine/scale.h"

struct ImageDimensions
{
  sf::Vector2f dimensions;
};

namespace image_dimensions
{
inline void synchronise_image_size_with_rect_size_plugin(AppCommands& app_commands)
{
  app_commands.add_system(Query<ImageDimensions const, Rectangle const, Scale>{}, [&](auto& view) {
    for (auto&& [_, image_dimensions, rectangle, scale] : view.each()) {
      scale.scale.x = rectangle.width_height.x / image_dimensions.dimensions.x;
      scale.scale.y = rectangle.width_height.y / image_dimensions.dimensions.y;
    }
  });
}
};// namespace image_dimensions
