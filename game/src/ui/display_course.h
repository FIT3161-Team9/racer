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

#include "game/src/game_state.h"
#include "game/src/ui/background.h"
#include "game/src/ui/vehicle_select.h"
#include "game/src/utils.h"

namespace display_course
{

void spawn_ui(AppCommands&);

enum class UIElement {};

/// This plugin implements the game's main screen
inline void plugin(AppCommands& app_commands)
{
  // Listen for the "enter" key
  app_commands.template add_system<Event::EventType::KeyReleased>(
    ResourceQuery<GameState>{}, Query<UIElement>{}, [&](auto& event, auto& resource_tuple, auto& view) {
      auto&& [_, game_state] = resource_tuple;

      // If the key that was pressed wasn't "enter", or the current screen isn't
      // the main screen, do nothing
      if (game_state.current_screen == GameState::CurrentScreen::DisplayCourse) {

        if (event.key_released.key == sf::Keyboard::Key::Enter) {
          game_state.current_screen = GameState::CurrentScreen::VehicleSelect1;

          for (auto&& [entity, _ui_element] : view.each()) { app_commands.destroy(entity); }

          vehicle_select::spawn_ui(app_commands);
          return true;
        }
      }
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
  using utils::u8;

  auto background = background::spawn(app_commands);

  background.add_component<UIElement>();

  auto title = app_commands.spawn()
                 .add_component<UIElement>()
                 .add_component<Text>(utils::INTER_BLACK, "RACER", u32(97), 2.5f)
                 .add_component<Colour>(colour::black())
                 .add_component<layout::Margin>(layout::Margin{ .top = 80.f, .left = 80.f });

  auto subtitle = app_commands.spawn()
                    .add_component<UIElement>()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "SIMULATION-BASED RACING", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .top = 40.f, .left = 80.f });

  auto select_label = app_commands.spawn()
                        .add_component<UIElement>()
                        .add_component<Text>(utils::INTER_SEMI_BOLD, "COURSE", u32(66), 0.85f)
                        .add_component<Colour>(colour::black())
                        .add_component<layout::Margin>(layout::Margin{ .top = -120.f, .left = 1600.f });


  auto Jungle_txt = app_commands.spawn()
                      .add_component<UIElement>()
                      .add_component<Text>(utils::INTER_SEMI_BOLD, "Jungle Name", u32(53), 0.85f)
                      .add_component<Colour>(colour::black())
                      .add_component<layout::Margin>(layout::Margin{ .top = 670.f, .left = 900.f });


  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ 1400.f, 550.f })
    .add_component<Colour>(colour::white())
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<ZIndex>(2)
    .add_component<Transform>(sf::Vector2f{ 0.f, 0.f });


  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ 80.f, 40.f })
    .add_component<Colour>(u8(255), u8(237), u8(237))
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<ZIndex>(3)
    .add_component<Transform>(sf::Vector2f{ -783.f, 425.f });

  auto prompt_1 = app_commands.spawn()
                    .add_component<UIElement>()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "USE THE ", u32(21), 0.85f)
                    .add_component<Colour>(colour::black());

  auto prompt_4 = app_commands.spawn()
                    .add_component<UIElement>()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, " ENTER ", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .left = 10.f });

  auto prompt_5 = app_commands.spawn()
                    .add_component<UIElement>()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "TO CONTINUE", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .left = 10.f });

  auto bottom_row =
    app_commands.spawn()
      .add_component<UIElement>()
      .template add_component<layout::Flex>(layout::Flex::Direction::Horizontal, layout::Flex::Alignment::End)
      .template add_component<Children>(std::vector{ prompt_1.entity(), prompt_4.entity(), prompt_5.entity() })
      .template add_component<layout::Margin>(layout::Margin{ .top = 58.f, .left = 80.f });

  app_commands.spawn()
    .add_component<UIElement>()
    .template add_component<layout::FlexRoot>()
    .template add_component<layout::Flex>(layout::Flex::Direction::Vertical, layout::Flex::Alignment::Start)
    .template add_component<Children>(std::vector{
      title.entity(),
      subtitle.entity(),
      select_label.entity(),
      Jungle_txt.entity(),
      bottom_row.entity(),
    });
}

};// namespace display_course
