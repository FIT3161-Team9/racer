#pragma once

#include <random>

#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"

#include "engine/app_commands.h"
#include "engine/circle.h"
#include "engine/colour.h"
#include "engine/outline.h"
#include "engine/rotation.h"
#include "engine/scale.h"
#include "engine/texture.h"
#include "engine/transform.h"
#include "engine/zindex.h"

#include "game/src/acceleration.h"
#include "game/src/camera.h"
#include "game/src/debug/deletable.h"
#include "game/src/debug/draggable.h"
#include "game/src/debug/selectable.h"
#include "game/src/gravity.h"
#include "game/src/maximum_velocity.h"
#include "game/src/vehicle.h"
#include "game/src/velocity.h"

namespace spawn_vehicle
{

inline void spawn(AppCommands& app_commands, sf::Vector2f const& location)
{
  const std::uint32_t seed = static_cast<std::uint32_t>(std::random_device{}());
  std::mt19937 generator(seed);
  std::uniform_int_distribution<std::int32_t> distribute(500.f, 800.f);

  auto const random_value = distribute(generator);


  app_commands.spawn()
    .add_component<Vehicle>(false)
    .add_component<Acceleration>(sf::Vector2f{ static_cast<float>(random_value), 0.f })
    .add_component<Fuel>(0.1f, 100.f, 100.f)
    .add_component<MaximumVelocity>(2000.f)
    .add_component<AffectedByGravity>()
    .add_component<camera::Target>()
    .add_component<Circle>(77.5f)
    .add_component<Colour>(colour::transparent())
    .add_component<debug::Draggable>()
    .add_component<Outline>(colour::transparent(), 2.f)
    .add_component<Rotation>(0.f)
    .add_component<Scale>(sf::Vector2f{ 1.f, 1.f })
    .add_component<debug::Selectable>()
    .add_component<debug::Deletable>()
    .add_component<Texture>("assets/Car1.png")
    .add_component<ZIndex>(2)
    .add_component<Transform>(location)
    .add_component<Velocity>(sf::Vector2f{ 0.f, 0.f });
}

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system<Event::EventType::KeyReleased>([&](auto& event) {
    if (event.key_released.key != sf::Keyboard::S) { return false; }

    spawn(app_commands, sf::Vector2f{ 0.f, 0.f });

    return false;
  });
}

}// namespace spawn_vehicle
