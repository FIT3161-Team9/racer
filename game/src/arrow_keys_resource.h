#pragma once

#include "engine/app_commands.h"

struct ArrowKeysResource
{
  bool left_pressed{ false };
  bool right_pressed{ false };
  bool up_pressed{ false };
  bool down_pressed{ false };
};

namespace arrow_keys_resource
{

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_resource<ArrowKeysResource>();

  // Handle arrow keys pressed
  app_commands.add_system<Event::EventType::KeyPressed>(ResourceQuery<ArrowKeysResource>{},
                                                        [](auto& event, auto& resources) {
                                                          auto&& [_, arrow_key_resource] = resources;
                                                          if (event.key_pressed.key == sf::Keyboard::Left) {
                                                            arrow_key_resource.left_pressed = true;
                                                          }
                                                          if (event.key_pressed.key == sf::Keyboard::Right) {
                                                            arrow_key_resource.right_pressed = true;
                                                          }
                                                          if (event.key_pressed.key == sf::Keyboard::Up) {
                                                            arrow_key_resource.up_pressed = true;
                                                          }
                                                          if (event.key_pressed.key == sf::Keyboard::Down) {
                                                            arrow_key_resource.down_pressed = true;
                                                          }
                                                          return false;
                                                        });

  // Handle arrow keys released
  app_commands.add_system<Event::EventType::KeyReleased>(ResourceQuery<ArrowKeysResource>{},
                                                         [](auto& event, auto& resources) {
                                                           auto&& [_, arrow_key_resource] = resources;
                                                           if (event.key_released.key == sf::Keyboard::Left) {
                                                             arrow_key_resource.left_pressed = false;
                                                           }
                                                           if (event.key_released.key == sf::Keyboard::Right) {
                                                             arrow_key_resource.right_pressed = false;
                                                           }
                                                           if (event.key_released.key == sf::Keyboard::Up) {
                                                             arrow_key_resource.up_pressed = false;
                                                           }
                                                           if (event.key_released.key == sf::Keyboard::Down) {
                                                             arrow_key_resource.down_pressed = false;
                                                           }
                                                           return false;
                                                         });
}

}// namespace arrow_keys_resource
