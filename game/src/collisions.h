#pragma once

#include <engine/app_commands.h>
#include <engine/circle.h>
#include <engine/rectangle.h>
#include <engine/transform.h>
#include <engine/vector_utils.h>
#include <memory>

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

inline bool point_circle(Circle const& circle, Transform const& circle_transform, sf::Vector2f const& point)
{
  return vector_utils::distance_between(circle_transform.value, point) <= circle.radius;
}

inline bool point_segment(Segment const& segment, sf::Vector2f const& point)
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

/// Given a segment and a circle, if the circle is colliding with the segment, return the
/// point on the segment that the circle is touching
inline std::unique_ptr<sf::Vector2f>
  segment_circle(Segment const& segment, Circle const& circle, Transform const& circle_transform)
{
  // If either end of the segment is within the circle, return true
  if (point_circle(circle, circle_transform, segment.start)) { return std::make_unique<sf::Vector2f>(segment.start); }
  if (point_circle(circle, circle_transform, segment.end)) { return std::make_unique<sf::Vector2f>(segment.end); }

  auto const segment_length = vector_utils::distance_between(segment.start, segment.end);
  if (segment_length == 0.f) { return nullptr; }

  auto const dot = ((circle_transform.value.x - segment.start.x) * (segment.end.x - segment.start.x)
                    + (circle_transform.value.y - segment.start.y) * (segment.end.y - segment.start.y))
                   / (segment_length * segment_length);

  // Find the closest point on the line
  float closest_x = segment.start.x + (dot * (segment.end.x - segment.start.x));
  float closest_y = segment.start.y + (dot * (segment.end.y - segment.start.y));
  auto const closest_point = sf::Vector2f{ closest_x, closest_y };

  if (!point_segment(segment, closest_point)) { return nullptr; }

  auto const distance = vector_utils::magnitude(vector_utils::minus(closest_point, circle_transform.value));

  if (distance <= circle.radius) { return std::make_unique<sf::Vector2f>(closest_point); }
  return nullptr;
}


struct RectangleCircleCollision
{
  sf::Vector2f normal;
  sf::Vector2f point_of_collision;
};

/// Given a rectangle and a circle, if they are colliding, return the normal of the edge of the rectangle
/// the circle is touching as well as the point of contact along that edge
inline std::unique_ptr<RectangleCircleCollision> rectangle_circle(Rectangle const& rectangle,
                                                                  Transform const& rectangle_transform,
                                                                  Rotation const& rectangle_rotation,
                                                                  Circle const& circle,
                                                                  Transform const& circle_transform)
{
  auto const segments = rectangle_utils::segments(rectangle, rectangle_transform, rectangle_rotation);

  // Work out which segment we collided with
  // Work out what coordinate we collided at (closest point on segment to center of circle)
  // Unit vector in the direction of the normal to the segment

  for (auto const segment : segments) {
    auto const collision_point = segment_circle(segment, circle, circle_transform);
    if (collision_point) {
      auto const normal =
        rotation::rotate_about_point(vector_utils::minus(segment.end, segment.start), { 0.f, 0.f }, 90.f);
      auto const normal_unit = vector_utils::scale_vector(normal, 1 / vector_utils::magnitude(normal));

      return std::make_unique<RectangleCircleCollision>(
        RectangleCircleCollision{ sf::Vector2f{ normal_unit.x, normal_unit.y }, *collision_point });
    }
  }

  if (point_rectangle(rectangle, rectangle_transform, rectangle_rotation, circle_transform.value)) {
    return std::make_unique<RectangleCircleCollision>(
      RectangleCircleCollision{ sf::Vector2f{ 0.f, 0.f }, sf::Vector2f{ 0.f, 0.f } });
  }
  return nullptr;
}

inline bool point_rectangle(Rectangle const& rectangle,
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
