#pragma once

#include <entt/entt.hpp>
#include <filesystem>
#include <vector>

#include <SFML/Graphics.hpp>

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

#include "game/src/count_down_timer.h"
#include "game/src/debug/spawn_vehicle.h"
#include "game/src/game_state.h"
#include "game/src/map.h"
#include "game/src/ui/background.h"
#include "game/src/ui/icon.h"
#include "game/src/ui/map_select.h"
#include "game/src/utils.h"

namespace main_menu
{

void spawn_ui(AppCommands&);

struct SelectedState
{
  int state;
};

enum class UIElement {};

/// This plugin implements the game's main screen
inline void plugin(AppCommands& app_commands)
{
  app_commands.add_resource<SelectedState>(0);
  app_commands.add_system<Event::EventType::KeyPressed>(ResourceQuery<GameState, SelectedState>{},
                                                        Query<Icon const, Transform>{},
                                                        [&](auto& event, auto& resource_tuple, auto& view) {
                                                          auto&& [_, game_state, selected_state] = resource_tuple;
                                                          if (game_state.current_screen
                                                              != GameState::CurrentScreen::MainMenu) {
                                                            return false;
                                                          }
                                                          if (event.key_pressed.key == sf::Keyboard::Key::Up) {
                                                            for (auto&& [_entity, icon, transform] : view.each()) {
                                                              transform.value.y = -120.f;
                                                              selected_state.state = 0;
                                                            }
                                                          }
                                                          if (event.key_pressed.key == sf::Keyboard::Key::Down) {
                                                            for (auto&& [_entity, icon, transform] : view.each()) {
                                                              transform.value.y = 70.f;
                                                              selected_state.state = 1;
                                                            }
                                                          }
                                                          return false;
                                                        });

  // Spawn the background
  auto const background = background::spawn(app_commands).entity();

  // Listen for the "enter" key
  app_commands.template add_system<Event::EventType::KeyReleased>(
    ResourceQuery<GameState, SelectedState>{},
    Query<UIElement>{},
    [&, background](auto& event, auto& resource_tuple, auto& view) {
      auto&& [_, game_state, selected_state] = resource_tuple;

      // If the key that was pressed wasn't "enter", or the current screen isn't
      // the main screen, do nothing
      if (game_state.current_screen == GameState::CurrentScreen::MainMenu) {
        if (event.key_released.key == sf::Keyboard::Key::Enter) {
          if (selected_state.state == 0) {
            // Delete all of the main_menu's ui elements
            view.each([&](auto entity, auto& ui_element) {
              (void)ui_element;
              app_commands.destroy(entity);
            });
            background::destroy(app_commands, background);

            game_state.current_screen = GameState::CurrentScreen::MapSelect;

            map_select::spawn_ui(app_commands);

            return true;
          }
          if (selected_state.state == 1) {
            app_commands.quit();
            return true;
          }
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
    .add_component<ZIndex>(2)
    .add_component<Transform>(sf::Vector2f{ -483.f, 425.f });

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

  auto play_button_label = app_commands.spawn()
                             .add_component<UIElement>()
                             .add_component<Text>(utils::INTER_SEMI_BOLD, "PLAY", u32(75), 0.85f)
                             .add_component<Colour>(colour::black());

  app_commands.spawn()
    .add_component<Triangle>(sf::Vector2f{ 50.f, 0.f }, sf::Vector2f{ 50.f, 60.f }, sf::Vector2f{ 90.f, 30.f })
    .add_component<UIElement>()
    .add_component<Colour>(colour::black())
    .add_component<Icon>()
    .add_component<Transform>(sf::Vector2f{ -610.f, -120.f })
    .add_component<ZIndex>(2);

  auto* selected_state = app_commands.get_resource<SelectedState>();
  selected_state->state = 0;

  auto play_button_row =
    app_commands.spawn()
      .add_component<UIElement>()
      .template add_component<layout::Flex>(layout::Flex::Direction::Horizontal, layout::Flex::Alignment::Center)
      .template add_component<layout::Height>(100.f)
      .template add_component<Children>(std::vector{ play_button_label.entity() })
      .template add_component<layout::Margin>(layout::Margin{ .top = 100.f, .bottom = 80.f, .left = 80.f });

  auto quit_button_label = app_commands.spawn()
                             .add_component<UIElement>()
                             .add_component<Text>(utils::INTER_SEMI_BOLD, "QUIT", u32(75), 0.85f)
                             .add_component<Colour>(colour::black());

  auto quit_button_row =
    app_commands.spawn()
      .add_component<UIElement>()
      .template add_component<layout::Flex>(layout::Flex::Direction::Horizontal, layout::Flex::Alignment::Center)
      .template add_component<layout::Height>(100.f)
      .template add_component<Children>(std::vector{ quit_button_label.entity() })
      .template add_component<layout::Margin>(layout::Margin{ .top = 0.f, .left = 80.f });

  auto prompt_1 = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "USE THE ", u32(21), 0.85f)
                    .add_component<UIElement>()
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
                    .add_component<Text>(utils::INTER_SEMI_BOLD, " ENTER ", u32(21), 0.85f)
                    .add_component<UIElement>()
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .left = 10.f });

  auto prompt_5 = app_commands.spawn()
                    .add_component<UIElement>()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "TO SELECT", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .left = 10.f });

  auto bottom_row =
    app_commands.spawn()
      .template add_component<layout::Flex>(layout::Flex::Direction::Horizontal, layout::Flex::Alignment::End)
      .add_component<UIElement>()
      .template add_component<Children>(std::vector{
        prompt_1.entity(),
        prompt_2.entity(),
        prompt_3.entity(),
        prompt_4.entity(),
        prompt_5.entity(),
      })
      .template add_component<layout::Margin>(layout::Margin{ .top = 327.f, .left = 80.f });

  app_commands.spawn()
    .template add_component<layout::FlexRoot>()
    .add_component<UIElement>()
    .template add_component<layout::Flex>(layout::Flex::Direction::Vertical, layout::Flex::Alignment::Start)
    .template add_component<Children>(std::vector{
      title.entity(),
      subtitle.entity(),
      play_button_row.entity(),
      quit_button_row.entity(),
      bottom_row.entity(),
    });
}


};// namespace main_menu
