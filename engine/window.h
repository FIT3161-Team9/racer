#pragma once

#include <SFML/Graphics.hpp>

namespace window
{

float const COORDINATE_SPACE_WIDTH = 2000;
float const COORDINATE_SPACE_HEIGHT = 1000;
auto const ASPECT_RATIO = COORDINATE_SPACE_WIDTH / COORDINATE_SPACE_HEIGHT;

sf::Vector2f to_screen_space(sf::Vector2f const& vec)
{
  return { vec.x + 0.5f * COORDINATE_SPACE_WIDTH, vec.y + 0.5f * COORDINATE_SPACE_HEIGHT };
}


sf::Vector2f from_screen_space(sf::Vector2f const& vec)
{
  return { vec.x - 0.5f * COORDINATE_SPACE_WIDTH, vec.y - 0.5f * COORDINATE_SPACE_HEIGHT };
}

void scale_view_to_window_size(sf::RenderWindow& window)
{
  auto const [width, height] = window.getSize();

  auto const scaled_height = static_cast<float>(width) / window::ASPECT_RATIO;

  // If the scaled height is greater than the height of the window, we want to scale
  // the width
  auto const target_width = scaled_height > height ? height * window::ASPECT_RATIO : static_cast<float>(width);
  auto const target_height = scaled_height > height ? height : scaled_height;

  auto const target_width_scale = target_width / width;
  auto const target_height_scale = target_height / height;

  sf::View view(sf::FloatRect(0, 0, window::COORDINATE_SPACE_WIDTH, window::COORDINATE_SPACE_HEIGHT));

  view.setViewport(
    { (1 - target_width_scale) * 0.5f, (1 - target_height_scale) * 0.5f, target_width_scale, target_height_scale });

  window.setView(view);
}
};// namespace window