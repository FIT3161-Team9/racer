#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <iostream>

#include "engine/app_commands.h"
#include "engine/colour.h"
#include "engine/query.h"
#include "engine/rectangle.h"
#include "engine/rotation.h"
#include "engine/transform.h"
#include "engine/vector_utils.h"

#include "./rectangle_utils.h"
#include "./velocity.h"


struct Car
{
  entt::entity back_wheel;
  entt::entity front_wheel;
};

namespace car
{

enum class FrontWheel;
enum class BackWheel;

void synchronise_car_with_wheels(AppCommands& app_commands,
                                 entt::entity car,
                                 entt::entity back_wheel,
                                 entt::entity front_wheel)
{
  std::cout << "Car entity: " << static_cast<int>(car) << ", wheels: " << static_cast<int>(back_wheel) << ", "
            << static_cast<int>(front_wheel) << "\n";
  auto* back_wheel_velocity = app_commands.component<Velocity>(back_wheel);
  auto* front_wheel_velocity = app_commands.component<Velocity>(front_wheel);
  auto* car_velocity = app_commands.component<Velocity>(car);
  assert(back_wheel_velocity);
  assert(front_wheel_velocity);
  assert(car_velocity);
  auto const& fastest_wheel_velocity =
    vector_utils::magnitude(back_wheel_velocity->value) > vector_utils::magnitude(front_wheel_velocity->value)
      ? back_wheel_velocity->value
      : front_wheel_velocity->value;

  front_wheel_velocity->value = fastest_wheel_velocity;
  back_wheel_velocity->value = fastest_wheel_velocity;
  car_velocity->value = fastest_wheel_velocity;

  auto* car_rectangle = app_commands.component<Rectangle>(car);
  auto* car_transform = app_commands.component<Transform>(car);
  auto* car_rotation = app_commands.component<Rotation>(car);

  auto const bottom_left_of_car = rectangle_utils::bottom_left_corner(*car_rectangle, *car_transform, *car_rotation);
  auto const bottom_right_of_car = rectangle_utils::bottom_right_corner(*car_rectangle, *car_transform, *car_rotation);

  auto* front_wheel_transform = app_commands.component<Transform>(front_wheel);
  auto* back_wheel_transform = app_commands.component<Transform>(back_wheel);
  assert(front_wheel_transform);
  assert(back_wheel_transform);

  front_wheel_transform->value = bottom_right_of_car;
  back_wheel_transform->value = bottom_left_of_car;
}

void plugin(AppCommands& app_commands)
{
  app_commands.add_system(Query<Car>{}, [&app_commands](auto& view) {
    for (auto&& [car_entity, car] : view.each()) {
      synchronise_car_with_wheels(app_commands, car_entity, car.back_wheel, car.front_wheel);
    }
  });
}

Entity spawn(AppCommands& app_commands)
{
  auto back_wheel = app_commands.spawn()
                      .add_component<BackWheel>()
                      .add_component<Circle>(30.f)
                      .add_component<Colour>(colour::red())
                      .add_component<Transform>(sf::Vector2f{ 0.f, 0.f })
                      .add_component<Velocity>(sf::Vector2f{ 0.f, 0.f });

  auto front_wheel = app_commands.spawn()
                       .add_component<FrontWheel>()
                       .add_component<Circle>(30.f)
                       .add_component<Colour>(colour::red())
                       .add_component<Transform>(sf::Vector2f{ 0.f, 0.f })
                       .add_component<Velocity>(sf::Vector2f{ 200.f, 0.f });

  return app_commands.spawn()
    .add_component<Rectangle>(sf::Vector2f{ 100.f, 50.f })
    .add_component<Colour>(colour::white())
    .add_component<Transform>(sf::Vector2f{ 0.f, 0.f })
    .add_component<Rotation>(0.f)
    .add_component<Velocity>(sf::Vector2f{ 0.f, 0.f })
    .add_component<Car>(back_wheel.entity(), front_wheel.entity());
}
}// namespace car
