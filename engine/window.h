#pragma once

#include <SFML/Graphics.hpp>

namespace window
{

/// The width of the app's coordinate space
float const COORDINATE_SPACE_WIDTH = 2000;
/// The height of the app's coordinate space
float const COORDINATE_SPACE_HEIGHT = 1000;
/// The aspect ratio of the app's coordinate space
auto const ASPECT_RATIO = COORDINATE_SPACE_WIDTH / COORDINATE_SPACE_HEIGHT;

/// Given a vector in SFML's coordinate system, translate it to the engine's coordinate system
///
/// SFML treats the top left of the screen as (0, 0), the engine treats the middle of the screen
/// as (0, 0)
inline sf::Vector2f to_screen_space(sf::Vector2f const& vec)
{
  return { vec.x + 0.5f * COORDINATE_SPACE_WIDTH, vec.y + 0.5f * COORDINATE_SPACE_HEIGHT };
}

/// Given a vector in the engine's coordinate system, translate it to SFML's coordinate system
///
/// SFML treats the top left of the screen as (0, 0), the engine treats the middle of the screen
/// as (0, 0)
inline sf::Vector2f from_screen_space(sf::Vector2f const& vec)
{
  return { vec.x - 0.5f * COORDINATE_SPACE_WIDTH, vec.y - 0.5f * COORDINATE_SPACE_HEIGHT };
}

/// Given a rectangle and a given aspect ratio, return the amount you would have to scale the rectangle by
/// in each dimension to have it equal to the given aspect ratio
inline sf::Vector2f scale_required_to_satisfy_aspect_ratio(sf::Vector2f const& rectangle, float const aspect_ratio)
{
  auto const width = rectangle.x;
  auto const height = rectangle.y;
  auto const scaled_height = width / aspect_ratio;

  auto const target_width = scaled_height > height ? height * aspect_ratio : width;
  auto const target_height = scaled_height > height ? height : scaled_height;

  auto const target_width_scale = target_width / width;
  auto const target_height_scale = target_height / height;

  return { target_width_scale, target_height_scale };
}

/// Scale the render view to fit the current window size. This basically ensures
/// that the aspect ratio of the view maintains consistent as the user resizes
/// the window
inline void scale_view_to_window_size(sf::RenderWindow& window)
{
  auto const [width, height] = window.getSize();
  auto [target_width_scale, target_height_scale] = scale_required_to_satisfy_aspect_ratio(
    { static_cast<float>(width), static_cast<float>(height) }, window::ASPECT_RATIO);

  sf::View view(sf::FloatRect(0, 0, window::COORDINATE_SPACE_WIDTH, window::COORDINATE_SPACE_HEIGHT));

  // Scale the width and height based on the aspect ratio
  auto const viewport = sf::Rect<float>{
    (1 - target_width_scale) * 0.5f, (1 - target_height_scale) * 0.5f, target_width_scale, target_height_scale
  };
  view.setViewport(viewport);

  window.setView(view);
}

};// namespace window
