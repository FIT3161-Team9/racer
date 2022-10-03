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

#include "engine/outline.h"
#include "engine/triangle.h"
#include "engine/zindex.h"
#include "game/src/count_down_timer.h"
#include "game/src/game_state.h"
#include "game/src/map.h"
#include "game/src/ui/background.h"
#include "game/src/utils.h"

namespace vehicle_select
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
      if (game_state.current_screen == GameState::CurrentScreen::VehicleSelect) {
        if (event.key_released.key == sf::Keyboard::Key::Enter) {

          for (auto&& [entity, _ui_element] : view.each()) { app_commands.destroy(entity); }

          auto const level = map::LEVELS[0];
          map::load_level(app_commands, level.c_str());
          count_down_timer::start(app_commands);

          game_state.current_screen = GameState::CurrentScreen::InLevel;

          return true;
        }
      }
      return false;
    });
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
                        .add_component<Text>(utils::INTER_SEMI_BOLD, "SELECT   ONE   PLAYER   VEHICLE", u32(66), 0.85f)
                        .add_component<Colour>(colour::black())
                        .add_component<layout::Margin>(layout::Margin{ .top = -120.f, .left = 700.f });
  auto state_label = app_commands.spawn()
                       .add_component<UIElement>()
                       .add_component<Text>(utils::INTER_SEMI_BOLD, "state", u32(53), 0.85f)
                       .add_component<Colour>(colour::black())
                       .add_component<layout::Margin>(layout::Margin{ .top = 80.f, .left = 1050.f });
  auto speed_state = app_commands.spawn()
                       .add_component<UIElement>()
                       .add_component<Text>(utils::INTER_SEMI_BOLD, "speed", u32(29), 0.85f)
                       .add_component<Colour>(colour::black())
                       .add_component<layout::Margin>(layout::Margin{ .top = 80.f, .left = 1050.f });
  auto acceleration_state = app_commands.spawn()
                              .add_component<UIElement>()
                              .add_component<Text>(utils::INTER_SEMI_BOLD, "acceleration", u32(29), 0.85f)
                              .add_component<Colour>(colour::black())
                              .add_component<layout::Margin>(layout::Margin{ .top = 130.f, .left = 1050.f });
  auto fuel_state = app_commands.spawn()
                      .add_component<UIElement>()
                      .add_component<Text>(utils::INTER_SEMI_BOLD, "fuel", u32(29), 0.85f)
                      .add_component<Colour>(colour::black())
                      .add_component<layout::Margin>(layout::Margin{ .top = 130.f, .left = 1050.f });
  auto jungle_txt = app_commands.spawn()
                      .add_component<UIElement>()
                      .add_component<Text>(utils::INTER_SEMI_BOLD, "Playing  On  Jungle", u32(53), 0.85f)
                      .add_component<Colour>(colour::black())
                      .add_component<layout::Margin>(layout::Margin{ .top = 130.f, .left = 1100.f });


  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ 700.f, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::black())
    .add_component<ZIndex>(3)
    .add_component<Transform>(sf::Vector2f{ 400.f, -80.f });

  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ 300.f, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::white())
    .add_component<ZIndex>(4)
    .add_component<Transform>(sf::Vector2f{ 200.f, -80.f });

  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ 700.f, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::black())
    .add_component<ZIndex>(3)
    .add_component<Transform>(sf::Vector2f{ 400.f, 75.f });

  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ 500.f, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::white())
    .add_component<ZIndex>(4)
    .add_component<Transform>(sf::Vector2f{ 300.f, 75.f });


  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ 700.f, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::black())
    .add_component<ZIndex>(3)
    .add_component<Transform>(sf::Vector2f{ 400.f, 215.f });

  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ 600.f, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::white())
    .add_component<ZIndex>(4)
    .add_component<Transform>(sf::Vector2f{ 350.f, 215.f });


  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ 625.f, 545.f })
    .add_component<Colour>(colour::white())
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<ZIndex>(2)
    .add_component<Transform>(sf::Vector2f{ -500.f, 50.f });

  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Triangle>(sf::Vector2f{ 50.f, 0.f }, sf::Vector2f{ 50.f, 120.f }, sf::Vector2f{ 110.f, 60.f })
    .add_component<Colour>(colour::black())
    .add_component<Transform>(sf::Vector2f{ -805.f, 100.f });

  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Triangle>(sf::Vector2f{ 50.f, 0.f }, sf::Vector2f{ 50.f, 120.f }, sf::Vector2f{ -10.f, 60.f })
    .add_component<Colour>(colour::black())
    .add_component<Transform>(sf::Vector2f{ -95.f, 100.f });

  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ 135.f, 40.f })
    .add_component<Colour>(u8(255), u8(237), u8(237))
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<ZIndex>(2)
    .add_component<Transform>(sf::Vector2f{ -753.f, 425.f });

  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ 80.f, 40.f })
    .add_component<Colour>(u8(255), u8(237), u8(237))
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<ZIndex>(3)
    .add_component<Transform>(sf::Vector2f{ -483.f, 425.f });

  auto prompt_1 = app_commands.spawn()
                    .add_component<UIElement>()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "USE THE ", u32(21), 0.85f)
                    .add_component<Colour>(colour::black());

  auto prompt_2 = app_commands.spawn()
                    .add_component<UIElement>()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, " ARROW KEY ", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .left = 10.f });

  auto prompt_3 = app_commands.spawn()
                    .add_component<UIElement>()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "TO NAVIGATE,", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .left = 10.f });

  auto prompt_4 = app_commands.spawn()
                    .add_component<UIElement>()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, " ENTER ", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .left = 10.f });

  auto prompt_5 = app_commands.spawn()
                    .add_component<UIElement>()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "TO SELECT", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .left = 10.f });

  auto bottom_row =
    app_commands.spawn()
      .add_component<UIElement>()
      .template add_component<layout::Flex>(layout::Flex::Direction::Horizontal, layout::Flex::Alignment::End)
      .template add_component<Children>(std::vector{
        prompt_1.entity(),
        prompt_2.entity(),
        prompt_3.entity(),
        prompt_4.entity(),
        prompt_5.entity(),
      })
      .template add_component<layout::Margin>(layout::Margin{ .top = 68.f, .left = 80.f });

  app_commands.spawn()
    .add_component<UIElement>()
    .template add_component<layout::FlexRoot>()
    .template add_component<layout::Flex>(layout::Flex::Direction::Vertical, layout::Flex::Alignment::Start)
    .template add_component<Children>(std::vector{
      title.entity(),
      subtitle.entity(),
      select_label.entity(),
      state_label.entity(),
      speed_state.entity(),
      acceleration_state.entity(),
      fuel_state.entity(),
      jungle_txt.entity(),
      bottom_row.entity(),
    });
}

};// namespace vehicle_select
