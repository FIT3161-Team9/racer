#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include <engine/rectangle.h>
#include <engine/rotation.h>
#include <engine/transform.h>

#include "segment.h"

namespace rectangle_utils
{

enum class Corner {
  TopRight,
  TopLeft,
  BottomRight,
  BottomLeft,
};

inline sf::Vector2f corner(Corner corner,
                           Rectangle const& rectangle,
                           Transform const& rectangle_transform,
                           Rotation const& rectangle_rotation)
{
  switch (corner) {
  case Corner::TopRight: {
    return rotation::rotate_about_point(sf::Vector2f{ rectangle_transform.value.x + rectangle.width_height.x * 0.5f,
                                                      rectangle_transform.value.y + rectangle.width_height.y * 0.5f },
                                        rectangle_transform.value,
                                        rectangle_rotation.degrees);
  }
  case Corner::TopLeft: {
    return rotation::rotate_about_point(sf::Vector2f{ rectangle_transform.value.x - rectangle.width_height.x * 0.5f,
                                                      rectangle_transform.value.y + rectangle.width_height.y * 0.5f },
                                        rectangle_transform.value,
                                        rectangle_rotation.degrees);
  }
  case Corner::BottomRight: {
    return rotation::rotate_about_point(sf::Vector2f{ rectangle_transform.value.x + rectangle.width_height.x * 0.5f,
                                                      rectangle_transform.value.y - rectangle.width_height.y * 0.5f },
                                        rectangle_transform.value,
                                        rectangle_rotation.degrees);
  }
  case Corner::BottomLeft: {
    return rotation::rotate_about_point(sf::Vector2f{ rectangle_transform.value.x - rectangle.width_height.x * 0.5f,
                                                      rectangle_transform.value.y - rectangle.width_height.y * 0.5f },
                                        rectangle_transform.value,
                                        rectangle_rotation.degrees);
  }
  }
}

/// The four corners of a rectangle
inline std::vector<sf::Vector2f>
  corners(Rectangle const& rectangle, Transform const& rectangle_transform, Rotation const& rectangle_rotation)
{
  auto const top_right = corner(Corner::TopRight, rectangle, rectangle_transform, rectangle_rotation);
  auto const top_left = corner(Corner::TopLeft, rectangle, rectangle_transform, rectangle_rotation);
  auto const bottom_right = corner(Corner::BottomRight, rectangle, rectangle_transform, rectangle_rotation);
  auto const bottom_left = corner(Corner::BottomLeft, rectangle, rectangle_transform, rectangle_rotation);

  return { top_right, top_left, bottom_right, bottom_left };
}

/// The four segments that make up a rectangle's outline
inline std::vector<Segment>
  segments(Rectangle const& rectangle, Transform const& rectangle_transform, Rotation const& rectangle_rotation)
{
  auto const top_right =
    rotation::rotate_about_point(sf::Vector2f{ rectangle_transform.value.x + rectangle.width_height.x * 0.5f,
                                               rectangle_transform.value.y + rectangle.width_height.y * 0.5f },
                                 rectangle_transform.value,
                                 rectangle_rotation.degrees);
  auto const top_left =
    rotation::rotate_about_point(sf::Vector2f{ rectangle_transform.value.x - rectangle.width_height.x * 0.5f,
                                               rectangle_transform.value.y + rectangle.width_height.y * 0.5f },
                                 rectangle_transform.value,
                                 rectangle_rotation.degrees);
  auto const bottom_right =
    rotation::rotate_about_point(sf::Vector2f{ rectangle_transform.value.x + rectangle.width_height.x * 0.5f,
                                               rectangle_transform.value.y - rectangle.width_height.y * 0.5f },
                                 rectangle_transform.value,
                                 rectangle_rotation.degrees);
  auto const bottom_left =
    rotation::rotate_about_point(sf::Vector2f{ rectangle_transform.value.x - rectangle.width_height.x * 0.5f,
                                               rectangle_transform.value.y - rectangle.width_height.y * 0.5f },
                                 rectangle_transform.value,
                                 rectangle_rotation.degrees);

  return { Segment{ .start = bottom_right, .end = bottom_left },
           Segment{ .start = bottom_left, .end = top_left },
           Segment{ .start = top_left, .end = top_right },
           Segment{ .start = top_right, .end = bottom_right } };
}
}// namespace rectangle_utils
