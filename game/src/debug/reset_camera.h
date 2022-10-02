#pragma once

#include "SFML/Window/Keyboard.hpp"

#include "engine/app_commands.h"
#include "engine/event.h"
#include "engine/query.h"
#include "engine/transform.h"

#include "game/src/spawn_transform.h"

namespace reset_camera
{

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system<Event::EventType::KeyReleased>(
    Query<SpawnTransform const, Transform>{}, [&](auto& event, auto& view) {
      if (event.key_released.key != sf::Keyboard::R) { return false; }

      view.each([](auto const& spawn_transform, auto& transform) { transform.value = spawn_transform.value; });

      return false;
    });
}

}// namespace reset_camera
