#pragma once

#include "SFML/Window/Keyboard.hpp"

#include "engine/app_commands.h"
#include "engine/event.h"
#include "engine/query.h"

#include "game/src/pause_state.h"

namespace debug::pausable
{

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system<Event::EventType::KeyReleased>(ResourceQuery<PauseState>{},
                                                         [&](auto& event, auto& resources) {
                                                           if (event.key_released.key != sf::Keyboard::P) {
                                                             return false;
                                                           }
                                                           auto&& [_, pause_state] = resources;
                                                           pause_state.paused = !pause_state.paused;
                                                           return true;
                                                         });
}

}// namespace debug::pausable
