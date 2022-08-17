#pragma once

#include <engine/app_commands.h>
#include <engine/circle.h>
#include <engine/rectangle.h>
#include <engine/transform.h>
#include <engine/vector_utils.h>

#include "rectangle_utils.h"
#include "segment.h"

namespace collisions
{
bool point_circle(Circle const& circle, Transform const& circle_transform, sf::Vector2f const& point);
bool point_segment(Segment const& segment, sf::Vector2f const& point);
bool point_rectangle(Rectangle const& rectangle,
                     Transform const& rectangle_transform,
                     Rotation const& rectangle_rotation,
                     sf::Vector2f const& point);

bool point_circle(Circle const& circle, Transform const& circle_transform, sf::Vector2f const& point)
{
  return vector_utils::distance_between(circle_transform.value, point) <= circle.radius;
}

bool point_segment(Segment const& segment, sf::Vector2f const& point)
{
  // Since float comparison isn't perfect, let's check equality
  // within a certain threshold
  auto const inaccuracy_threshold = 0.1f;

  auto const distance_to_start = vector_utils::distance_between(segment.start, point);
  auto const distance_to_end = vector_utils::distance_between(segment.end, point);
  auto const segment_len = vector_utils::distance_between(segment.start, segment.end);

  return (distance_to_end + distance_to_start) >= segment_len - inaccuracy_threshold
         && (distance_to_end + distance_to_start) <= segment_len + inaccuracy_threshold;
}

bool segment_circle(Segment const& segment, Circle const& circle, Transform const& circle_transform)
{
  // If either end of the segment is within the circle, return true
  if (point_circle(circle, circle_transform, segment.start) || point_circle(circle, circle_transform, segment.end)) {
    return true;
  }

  auto const segment_length = vector_utils::distance_between(segment.start, segment.end);
  if (segment_length == 0.f) { return false; }

  auto const dot = ((circle_transform.value.x - segment.start.x) * (segment.end.x - segment.start.x)
                    + (circle_transform.value.y - segment.start.y) * (segment.end.y - segment.start.y))
                   / (segment_length * segment_length);

  // Find the closest point on the line
  float closest_x = segment.start.x + (dot * (segment.end.x - segment.start.x));
  float closest_y = segment.start.y + (dot * (segment.end.y - segment.start.y));
  auto const closest_point = sf::Vector2f{ closest_x, closest_y };

  if (!point_segment(segment, closest_point)) { return false; }

  auto const distance = vector_utils::magnitude(vector_utils::minus(closest_point, circle_transform.value));

  return distance <= circle.radius;
}

bool rectangle_circle(Rectangle const& rectangle,
                      Transform const& rectangle_transform,
                      Rotation const& rectangle_rotation,
                      Circle const& circle,
                      Transform const& circle_transform)
{
  auto const segments = rectangle_utils::segments(rectangle, rectangle_transform, rectangle_rotation);

  std::cout << "SEGMENTS:\n\n";
  for (auto const segment : segments) {
    std::cout << "Segment: start(" << segment.start.x << ", " << segment.start.y << "), end(" << segment.end.x << ", "
              << segment.end.y << ")\n";
    if (segment_circle(segment, circle, circle_transform)) {
      std::cout << "\nCOLLISION: above segment with circle(" << circle_transform.value.x << ", "
                << circle_transform.value.y << ")\n";
      return true;
    }
  }

  return point_rectangle(rectangle, rectangle_transform, rectangle_rotation, circle_transform.value);
}

bool point_rectangle(Rectangle const& rectangle,
                     Transform const& rectangle_transform,
                     Rotation const& rectangle_rotation,
                     sf::Vector2f const& point)
{
  auto corners = rectangle_utils::corners(rectangle, rectangle_transform, rectangle_rotation);
  for (auto i = 0; i < 4; i++) {
    corners[i] = rotation::rotate_about_point(corners[i], sf::Vector2f{ 0.f, 0.f }, -rectangle_rotation.degrees);
  }
  auto const point_rotated_back =
    rotation::rotate_about_point(point, sf::Vector2f{ 0.f, 0.f }, -rectangle_rotation.degrees);

  auto const left_side = std::min(std::min(corners[0].x, corners[1].x), std::min(corners[2].x, corners[3].x));
  auto const right_side = std::max(std::max(corners[0].x, corners[1].x), std::max(corners[2].x, corners[3].x));
  auto const bottom_side = std::min(std::min(corners[0].y, corners[1].y), std::min(corners[2].y, corners[3].y));
  auto const top_side = std::max(std::max(corners[0].y, corners[1].y), std::max(corners[2].y, corners[3].y));

  return point_rotated_back.x >= left_side && point_rotated_back.x <= right_side && point_rotated_back.y >= bottom_side
         && point_rotated_back.y <= top_side;
}

};// namespace collisions
