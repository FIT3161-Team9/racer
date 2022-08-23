#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
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

#include "../game_state.h"
#include "../utils.h"

namespace main_menu
{

void spawn_ui(AppCommands&);
void destroy_ui(AppCommands&, entt::entity flex_container, Children&);

/// This plugin implements the game's splash screen
inline void plugin(AppCommands& app_commands)
{
  // Spawn the background
  {
    using utils::u8;
    app_commands.spawn()
      .add_component<Rectangle>(sf::Vector2f{ window::COORDINATE_SPACE_WIDTH, window::COORDINATE_SPACE_HEIGHT })
      .add_component<Colour>(u8(255), u8(237), u8(237))
      .add_component<Transform>(sf::Vector2f{ 0.f, 0.f });
  }

  // Listen for the "enter" key
  app_commands.template add_system<Event::EventType::KeyReleased>(
    ResourceQuery<GameState>{}, Query<layout::Flex>{}, [&](auto& event, auto& resource_tuple, auto& flex_query) {
      auto&& [_, game_state] = resource_tuple;

      // If the key that was pressed wasn't "enter", or the current screen isn't
      // the splash screen, do nothing
      if (event.key_released.key != sf::Keyboard::Key::Enter
          || game_state.current_screen != GameState::CurrentScreen::MainMenu) {
        return;
      }

      game_state.current_screen = GameState::CurrentScreen::DisplayCrouse;
      auto flex_container = *flex_query.begin();
      destroy_ui(app_commands, flex_container, *app_commands.component<Children>(flex_container));

      // TODO: Spawn UI for main menu
    });
}

/// Destroy the UI for the splash screen
inline void destroy_ui(AppCommands& app_commands, entt::entity flex_container, Children& children)
{
  for (auto child : children.children) { app_commands.destroy(child); }
  app_commands.destroy(flex_container);
}

/// Create the UI for ths splash screen
inline void spawn_ui(AppCommands& app_commands)
{
  using utils::u32;
  auto title = app_commands.spawn()
                 .add_component<Text>(utils::INTER_BLACK, "1", u32(97), 2.5f)
                 .add_component<Colour>(colour::black())
                 .add_component<layout::Margin>(layout::margin_left(50.f));

  auto subtitle = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "2-BASED RACING", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::margin_left(50.f));

  auto prompt = app_commands.spawn()
                  .add_component<Text>(utils::INTER_SEMI_BOLD, "PRESS 3 BUTTON TO START", u32(28), 2.5f)
                  .add_component<Colour>(colour::black())
                  .add_component<layout::Margin>(layout::margin_left(50.f));

  app_commands.spawn()
    .template add_component<layout::Flex>(layout::Flex::Direction::Horizontal, layout::Flex::Alignment::End)
    .template add_component<Children>(std::vector{ title.entity(), subtitle.entity(), prompt.entity() });
}

};// namespace main_menu
