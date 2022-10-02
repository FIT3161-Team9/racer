#pragma once

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
#include "game/src/velocity.h"

namespace spawn_vehicle
{

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system<Event::EventType::KeyReleased>([&](auto& event) {
    if (event.key_released.key != sf::Keyboard::S) { return false; }
    app_commands.spawn()
      .add_component<Acceleration>(sf::Vector2f{ 600.f, 0.f })
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
      .add_component<Transform>(sf::Vector2f{ 0.f, 0.f })
      .add_component<Velocity>(sf::Vector2f{ 0.f, 0.f });

    return false;
  });
}

}// namespace spawn_vehicle
