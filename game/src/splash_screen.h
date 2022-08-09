#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <vector>

#include <engine/app_commands.h>
#include <engine/children.h>
#include <engine/colour.h>
#include <engine/event.h>
#include <engine/layout.h>
#include <engine/query.h>
#include <engine/rectangle.h>
#include <engine/text.h>
#include <engine/transform.h>
#include <engine/window.h>

#include "./game_state.h"


namespace splash_screen
{

inline void plugin(AppCommands& app_commands)
{
  // Listen for enter key
  app_commands.template add_system<Event::EventType::KeyReleased>(
    ResourceQuery<GameState>{}, Query<layout::Flex>{}, [&](auto& event, auto& resource_tuple, auto& flex_query) {
      auto&& [_, game_state] = resource_tuple;
      if (event.key_released.key == sf::Keyboard::Key::Enter
          && game_state.current_screen == GameState::CurrentScreen::SplashScreen) {
        std::cout << "Would transition to start screen\n";
        // update the game state
        // destroy the splash screen UI
        // create the main menu UI
        for (auto&& [flex_entity, _] : flex_query.each()) {
          auto* children = app_commands.component<Children>(flex_entity);
          if (children) {
            for (auto child : children->children) { app_commands.destroy(child); }
          }
          app_commands.destroy(flex_entity);
        }
      }
    });

  // Spawn UI
  app_commands.spawn()
    .template add_component<Rectangle>(sf::Vector2f{ window::COORDINATE_SPACE_WIDTH, window::COORDINATE_SPACE_HEIGHT })
    .template add_component<Colour>(static_cast<uint8_t>(255), static_cast<uint8_t>(237), static_cast<uint8_t>(237))
    .template add_component<Transform>(sf::Vector2f{ 0.f, 0.f });

  auto title = app_commands.spawn()
                 .template add_component<Text>(
                   std::filesystem::path("./assets/Inter-Black.ttf").string(), "RACER", static_cast<uint32_t>(97), 2.5f)
                 .template add_component<Colour>(colour::black())
                 .template add_component<layout::Margin>(layout::margin_top(350.f));

  auto subtitle = app_commands.spawn()
                    .template add_component<Text>(std::filesystem::path("./assets/Inter-SemiBold.ttf").string(),
                                                  "SIMULATION-BASED RACING",
                                                  static_cast<uint32_t>(21),
                                                  0.85f)
                    .template add_component<Colour>(colour::black())
                    .template add_component<layout::Margin>(layout::margin_top(45.f));

  auto prompt = app_commands.spawn()
                  .template add_component<Text>(std::filesystem::path("./assets/Inter-SemiBold.ttf").string(),
                                                "PRESS ENTER BUTTON TO START",
                                                static_cast<uint32_t>(28),
                                                2.5f)
                  .template add_component<Colour>(colour::black())
                  .template add_component<layout::Margin>(layout::margin_top(100.f));

  app_commands.spawn()
    .template add_component<layout::Flex>(layout::Flex::Direction::Vertical, layout::Flex::Alignment::Center)
    .template add_component<Children>(std::vector{ title.entity(), subtitle.entity(), prompt.entity() });
}

};// namespace splash_screen
