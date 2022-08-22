#pragma once

#include "engine/app_commands.h"
#include "engine/circle.h"
#include "engine/query.h"
#include "engine/rectangle.h"
#include "engine/rotation.h"
#include "engine/timing.h"
#include "engine/vector_utils.h"

#include "game/src/collisions.h"
#include "game/src/velocity.h"

enum class Ground {};

namespace ground
{


inline void print_vector(sf::Vector2f const& vec) { std::cout << "(x=" << vec.x << ", y=" << vec.y << ")"; }

inline void circle_ground_collision_plugin(AppCommands& app_commands)
{
  auto const handle = [](auto& resources, auto& circle_view, auto& ground_view) {
    auto&& [_, elapsed_time] = resources;
    for (auto&& [_ground_entity, ground_rectangle, ground_transform, ground_rotation, _ground] : ground_view.each()) {
      for (auto&& [_circle_entity, circle_velocity, circle_transform, circle] : circle_view.each()) {
        auto const collision_result =
          collisions::rectangle_circle(ground_rectangle, ground_transform, ground_rotation, circle, circle_transform);

        if (collision_result) {
          std::cout << "Collision test for circle at: ";
          print_vector(circle_transform.value);
          std::cout << "\nGround at: ";
          print_vector(ground_transform.value);
          std::cout << "(rotation: " << ground_rotation.degrees << ")\n";


          auto const depth_of_collision = circle.radius
                                          - vector_utils::magnitude(vector_utils::minus(
                                            collision_result->point_of_collision, circle_transform.value));

          std::cout << "Depth of collision: " << depth_of_collision << "\n";

          auto const depth_of_collision_in_direction_of_velocity = rotation::rotate_about_point(
            vector_utils::scale_vector(
              vector_utils::scale_vector(circle_velocity.value, 1 / vector_utils::magnitude(circle_velocity.value)),
              depth_of_collision),
            sf::Vector2f{ 0.f, 0.f },
            180.f);

          std::cout << "Depth of collision in direction of velocity: ";
          print_vector(depth_of_collision_in_direction_of_velocity);
          std::cout << "\n";

          circle_transform.value.x += depth_of_collision_in_direction_of_velocity.x;
          circle_transform.value.y += depth_of_collision_in_direction_of_velocity.y;

          std::cout << "Updated transform\n";
          if (collisions::rectangle_circle(
                ground_rectangle, ground_transform, ground_rotation, circle, circle_transform)) {
            std::cout << "Still colliding after updating transform...\n";
          }

          const auto dot = vector_utils::dot_product(circle_velocity.value, collision_result->normal);
          const auto new_velocity = vector_utils::scale_vector(
            vector_utils::minus(circle_velocity.value, vector_utils::scale_vector(collision_result->normal, 2 * dot)),
            0.75f);

          std::cout << "New velocity: ";
          print_vector(new_velocity);
          std::cout << "\n\n";

          circle_velocity.value = new_velocity;

          std::cout << "Intermediate transform: ";
          print_vector(circle_transform.value);
          std::cout << "\n";
          circle_transform.value.x += new_velocity.x * elapsed_time.elapsed_milliseconds * 0.001f;
          circle_transform.value.y += new_velocity.y * elapsed_time.elapsed_milliseconds * 0.001f;

          std::cout << "Final transform: ";
          print_vector(circle_transform.value);
          std::cout << "\n";
        }
      }
    }
  };

  app_commands.add_system(ResourceQuery<timing::ElapsedTime>{},
                          Query<Velocity, Transform, Circle>{},
                          Query<Rectangle, Transform, Rotation, Ground>{},
                          handle);
}

}// namespace ground
