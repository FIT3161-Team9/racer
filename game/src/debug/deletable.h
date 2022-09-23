#pragma once

#include "engine/app_commands.h"
#include "engine/event.h"
#include "engine/query.h"

#include "./selectable.h"

namespace debug
{

enum class Deletable {};

namespace deletable
{

  inline void plugin(AppCommands& app_commands)
  {
    app_commands.add_system<Event::EventType::KeyReleased>(
      Query<Selectable const, Deletable const>{}, [&](auto& event, auto& view) {
        if (!event.key_released.control || event.key_released.key != sf::Keyboard::BackSpace) { return false; }
        view.each([&](auto const deletable_entity, auto const& selectable, auto const& deletable) {
          (void)deletable;
          if (!selectable.selected) { return; }
          app_commands.destroy(deletable_entity);
        });

        return true;
      });
  }

}// namespace deletable

}// namespace debug
