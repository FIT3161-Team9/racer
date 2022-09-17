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
#include <engine/outline.h>
#include <engine/query.h>
#include <engine/rectangle.h>
#include <engine/text.h>
#include <engine/transform.h>
#include <engine/triangle.h>
#include <engine/window.h>
#include <engine/zindex.h>

#include "../game_state.h"
#include "../utils.h"
#include "./background.h"
#include "./icon.h"
#include "./vehicle_select.h"

namespace main_menu
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
      // auto icon
      if (event.key_released.key == sf::Keyboard::Key::Up) { app_commands.destroy(icon_change(start).entity()); }
      // If the key that was pressed wasn't "enter", or the current screen isn't
      // the main screen, do nothing
      if (event.key_released.key != sf::Keyboard::Key::Enter
          || game_state.current_screen != GameState::CurrentScreen::MainMenu) {
        return false;
      }

      game_state.current_screen = GameState::CurrentScreen::VehicleSelect;
      auto flex_container = *flex_query.begin();
      destroy_ui(app_commands, flex_container, *app_commands.component<Children>(flex_container));

      vehicle_select::spawn_ui(app_commands);
      return true;
    });
}

/// Destroy the UI for the main screen
inline void destroy_ui(AppCommands& app_commands, entt::entity flex_container, Children& children)
{
  for (auto child : children.children) { app_commands.destroy(child); }
  app_commands.destroy(flex_container);
}
inline void destroy_ui2(AppCommands& app_commands, entt::entity flex_container, Children& children) {}

/// Create the UI for ths main screen
inline void spawn_ui(AppCommands& app_commands)
{
  using utils::u32;
  using utils::u8;

  auto outline_p2 = app_commands.spawn()
                      .add_component<Rectangle>(sf::Vector2f{ 135.f, 40.f })
                      .add_component<Colour>(u8(255), u8(237), u8(237))
                      .add_component<Outline>(colour::black(), 2.2f)
                      .add_component<ZIndex>(2)
                      .add_component<Transform>(sf::Vector2f{ -753.f, 425.f });

  auto outline_p4 = app_commands.spawn()
                      .add_component<Rectangle>(sf::Vector2f{ 80.f, 40.f })
                      .add_component<Colour>(u8(255), u8(237), u8(237))
                      .add_component<Outline>(colour::black(), 2.2f)
                      .add_component<ZIndex>(2)
                      .add_component<Transform>(sf::Vector2f{ -483.f, 425.f });

  auto outline_p6 = app_commands.spawn()
                      .add_component<Rectangle>(sf::Vector2f{ 56.f, 40.f })
                      .add_component<Colour>(u8(255), u8(237), u8(237))
                      .add_component<Outline>(colour::black(), 2.2f)
                      .add_component<ZIndex>(2)
                      .add_component<Transform>(sf::Vector2f{ -231.f, 425.f });

  auto title = app_commands.spawn()
                 .add_component<Text>(utils::INTER_BLACK, "RACER", u32(97), 2.5f)
                 .add_component<Colour>(colour::black())
                 .add_component<layout::Margin>(layout::Margin{ .top = 80.f, .left = 80.f });

  auto subtitle = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "SIMULATION-BASED RACING", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .top = 40.f, .left = 80.f });

  auto play_button_label = app_commands.spawn()
                             .add_component<Text>(utils::INTER_SEMI_BOLD, "PLAY", u32(75), 0.85f)
                             .add_component<Colour>(colour::black());

  auto play_button_icon =
    app_commands.spawn()
      .add_component<Triangle>(sf::Vector2f{ 50.f, 0.f }, sf::Vector2f{ 50.f, 60.f }, sf::Vector2f{ 90.f, 30.f })
      .add_component<Colour>(colour::black())
      .add_component<Icon>(0)
      .add_component<Transform>(sf::Vector2f{ -610.f, -120.f });

  auto play_button_row =
    app_commands.spawn()
      .template add_component<layout::Flex>(layout::Flex::Direction::Horizontal, layout::Flex::Alignment::Center)
      .template add_component<layout::Height>(100.f)
      .template add_component<Children>(std::vector{ play_button_label.entity(), play_button_icon.entity() })
      .template add_component<layout::Margin>(layout::Margin{ .top = 140.f, .left = 80.f });

  auto quit_button_label = app_commands.spawn()
                             .add_component<Text>(utils::INTER_SEMI_BOLD, "QUIT", u32(75), 0.85f)
                             .add_component<Colour>(colour::black());

  auto quit_button_row =
    app_commands.spawn()
      .template add_component<layout::Flex>(layout::Flex::Direction::Horizontal, layout::Flex::Alignment::Center)
      .template add_component<layout::Height>(100.f)
      .template add_component<Children>(std::vector{ quit_button_label.entity() })
      .template add_component<layout::Margin>(layout::Margin{ .top = 0.f, .left = 80.f });

  auto prompt_1 = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "USE THE ", u32(21), 0.85f)
                    .add_component<Colour>(colour::black());

  auto prompt_2 = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, " ARROW KEY ", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .left = 10.f });

  auto prompt_3 = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "TO NAVIGATE,", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .left = 10.f });

  auto prompt_4 = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, " ENTER ", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .left = 10.f });

  auto prompt_5 = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "TO SELECT, AND", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .left = 10.f });

  auto prompt_6 = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, " ESC ", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .left = 10.f });

  auto prompt_7 = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "TO GO BACK", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .left = 10.f });

  auto bottom_row =
    app_commands.spawn()
      .template add_component<layout::Flex>(layout::Flex::Direction::Horizontal, layout::Flex::Alignment::End)
      .template add_component<Children>(std::vector{ prompt_1.entity(),
                                                     prompt_2.entity(),
                                                     prompt_3.entity(),
                                                     prompt_4.entity(),
                                                     prompt_5.entity(),
                                                     prompt_6.entity(),
                                                     prompt_7.entity() })
      .template add_component<layout::Margin>(layout::Margin{ .top = 347.f, .left = 80.f });

  app_commands.spawn()
    .template add_component<layout::FlexRoot>()
    .template add_component<layout::Flex>(layout::Flex::Direction::Vertical, layout::Flex::Alignment::Start)
    .template add_component<Children>(std::vector{ title.entity(),
                                                   subtitle.entity(),
                                                   play_button_row.entity(),
                                                   quit_button_row.entity(),
                                                   bottom_row.entity(),
                                                   //  quit_button_icon.entity(),
                                                   play_button_icon.entity(),
                                                   outline_p2.entity(),
                                                   outline_p4.entity(),
                                                   outline_p6.entity() });
}


};// namespace main_menu
