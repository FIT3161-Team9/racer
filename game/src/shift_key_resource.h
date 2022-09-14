#pragma once

#include "engine/app_commands.h"

struct ShiftKeyResource
{
  bool shift_pressed{ false };
};

namespace shift_key_resource
{

auto const shift_pressed_system = [](auto& event, auto& resources) -> bool {
  auto&& [_, shift_key_resource] = resources;
  if (event.key_pressed.key == sf::Keyboard::LShift || event.key_pressed.key == sf::Keyboard::RShift) {
    shift_key_resource.shift_pressed = true;
  }
  return false;
};

auto const shift_released_system = [](auto& event, auto& resources) {
  auto&& [_, shift_key_resource] = resources;
  if (event.key_released.key == sf::Keyboard::LShift || event.key_released.key == sf::Keyboard::RShift) {
    shift_key_resource.shift_pressed = false;
  }
  return false;
};

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_resource<ShiftKeyResource>();

  // Handle press shift key
  app_commands.add_system<Event::EventType::KeyPressed>(ResourceQuery<ShiftKeyResource>{}, shift_pressed_system);

  // Handle release shift key
  app_commands.add_system<Event::EventType::KeyReleased>(ResourceQuery<ShiftKeyResource>{}, shift_released_system);
}
}// namespace shift_key_resource
