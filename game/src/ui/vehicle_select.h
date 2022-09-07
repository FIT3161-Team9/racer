#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <filesystem>
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
#include "./background.h"

namespace vehicle_select
{

void spawn_ui(AppCommands&);
void destroy_ui(AppCommands&, entt::entity flex_container, Children&);

/// This plugin implements the game's main screen
inline void plugin(AppCommands& app_commands)
{
  // Listen for the "enter" key
  app_commands.template add_system<Event::EventType::KeyReleased>(
    ResourceQuery<GameState>{}, Query<layout::Flex>{}, [&](auto& event, auto& resource_tuple, auto& flex_query) {
      auto&& [_, game_state] = resource_tuple;

      // If the key that was pressed wasn't "enter", or the current screen isn't
      // the main screen, do nothing
      if (event.key_released.key != sf::Keyboard::Key::Enter
          || game_state.current_screen != GameState::CurrentScreen::VehicleSelect) {
        return false;
      }

      game_state.current_screen = GameState::CurrentScreen::ResultScreen;
      auto flex_container = *flex_query.begin();
      destroy_ui(app_commands, flex_container, *app_commands.component<Children>(flex_container));

      return false;
    });
}

/// Destroy the UI for the main screen
inline void destroy_ui(AppCommands& app_commands, entt::entity flex_container, Children& children)
{
  for (auto child : children.children) { app_commands.destroy(child); }
  app_commands.destroy(flex_container);
}

/// Create the UI for ths main screen
inline void spawn_ui(AppCommands& app_commands)
{
  using utils::u32;
  auto title = app_commands.spawn()
                 .add_component<Text>(utils::INTER_BLACK, "RACER", u32(97), 2.5f)
                 .add_component<Colour>(colour::black())
                 .add_component<layout::Margin>(layout::Margin{ .top = 80.f, .left = 80.f });

  auto subtitle = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "SIMULATION-BASED RACING", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .top = 40.f, .left = 80.f });
  
  auto prompt_1 = app_commands.spawn()
                  .add_component<Text>(utils::INTER_SEMI_BOLD, "USE THE ", u32(21), 0.85f)
                  .add_component<Colour>(colour::black());

  auto prompt_2 = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, " ARROW KEY ", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{.left = 10.f });

  auto prompt_3 = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "TO NAVIGATE,", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{.left = 10.f });

  auto prompt_4 = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, " ENTER ", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{.left = 10.f });
  
  auto prompt_5 = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "TO SELECT, AND", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{.left = 10.f });

  auto prompt_6 = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, " ESC ", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{.left = 10.f });

  auto prompt_7 = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "TO GO BACK", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{.left = 10.f });
                    
  
  auto bottom_row = app_commands.spawn()
      .template add_component<layout::Flex>(layout::Flex::Direction::Horizontal, layout::Flex::Alignment::End)
      .template add_component<Children>(std::vector{ prompt_1.entity(), prompt_2.entity(),prompt_3.entity(), prompt_4.entity(),prompt_5.entity(), prompt_6.entity(), prompt_7.entity() })
      .template add_component<layout::Margin>(layout::Margin{.top = 350.f, .left = 80.f});
  
  app_commands.spawn()
    .template add_component<layout::FlexRoot>()
    .template add_component<layout::Flex>(layout::Flex::Direction::Vertical, layout::Flex::Alignment::Start)
    .template add_component<Children>(
      std::vector{ title.entity(), subtitle.entity(), bottom_row.entity()});
}

};// namespace main_menu
