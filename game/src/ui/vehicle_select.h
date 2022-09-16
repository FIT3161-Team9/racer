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
#include "./result_screen.h"

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
      result_screen::spawn_ui(app_commands);
      return true;
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
  auto title = app_commands.spawn()
                 .add_component<Text>(utils::INTER_BLACK, "RACER", u32(97), 2.5f)
                 .add_component<Colour>(colour::black())
                 .add_component<layout::Margin>(layout::Margin{ .top = 80.f, .left = 80.f });

  auto subtitle = app_commands.spawn()
                    .add_component<Text>(utils::INTER_SEMI_BOLD, "SIMULATION-BASED RACING", u32(21), 0.85f)
                    .add_component<Colour>(colour::black())
                    .add_component<layout::Margin>(layout::Margin{ .top = 40.f, .left = 80.f });
  
  auto select_label = app_commands.spawn()
                             .add_component<Text>(utils::INTER_SEMI_BOLD, "SELECT   ONE   PLAYER   VEHICLE", u32(66), 0.85f)
                             .add_component<Colour>(colour::black())
                             .add_component<layout::Margin>(layout::Margin{ .top = -120.f, .left = 700.f });
  auto state_label = app_commands.spawn()
                             .add_component<Text>(utils::INTER_SEMI_BOLD, "state", u32(53), 0.85f)
                             .add_component<Colour>(colour::black())
                             .add_component<layout::Margin>(layout::Margin{ .top = 80.f, .left = 1050.f });
  auto speed_state = app_commands.spawn()
                             .add_component<Text>(utils::INTER_SEMI_BOLD, "speed", u32(29), 0.85f)
                             .add_component<Colour>(colour::black())
                             .add_component<layout::Margin>(layout::Margin{ .top = 80.f, .left = 1050.f });
  auto acceleration_state = app_commands.spawn()
                             .add_component<Text>(utils::INTER_SEMI_BOLD, "acceleration", u32(29), 0.85f)
                             .add_component<Colour>(colour::black())
                             .add_component<layout::Margin>(layout::Margin{ .top = 130.f, .left = 1050.f });
  auto fuel_state = app_commands.spawn()
                             .add_component<Text>(utils::INTER_SEMI_BOLD, "fuel", u32(29), 0.85f)
                             .add_component<Colour>(colour::black())
                             .add_component<layout::Margin>(layout::Margin{ .top = 130.f, .left = 1050.f });
  auto Jungle_txt = app_commands.spawn()
                             .add_component<Text>(utils::INTER_SEMI_BOLD, "Playing  On  Jungle", u32(53), 0.85f)
                             .add_component<Colour>(colour::black())
                             .add_component<layout::Margin>(layout::Margin{ .top = 130.f, .left = 1100.f });
  
  
  
  
  auto state_speed= app_commands.spawn()
    .add_component<Rectangle>(sf::Vector2f{ 700.f, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::black())
    .add_component<ZIndex>(3)
    .add_component<Transform>(sf::Vector2f{ 400.f, -80.f });
  
  auto vehicle_speed= app_commands.spawn()
    .add_component<Rectangle>(sf::Vector2f{ 300.f, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::white())
    .add_component<ZIndex>(4)
    .add_component<Transform>(sf::Vector2f{ 200.f, -80.f });
  
  auto state_acceleration= app_commands.spawn()
    .add_component<Rectangle>(sf::Vector2f{ 700.f, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::black())
    .add_component<ZIndex>(3)
    .add_component<Transform>(sf::Vector2f{ 400.f, 75.f });
  
  auto vehicle_acceleration= app_commands.spawn()
    .add_component<Rectangle>(sf::Vector2f{ 500.f, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::white())
    .add_component<ZIndex>(4)
    .add_component<Transform>(sf::Vector2f{ 300.f, 75.f });

  
  auto state_feul= app_commands.spawn()
    .add_component<Rectangle>(sf::Vector2f{ 700.f, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::black())
    .add_component<ZIndex>(3)
    .add_component<Transform>(sf::Vector2f{ 400.f, 215.f });

  auto vehicle_feul= app_commands.spawn()
    .add_component<Rectangle>(sf::Vector2f{ 600.f, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::white())
    .add_component<ZIndex>(4)
    .add_component<Transform>(sf::Vector2f{ 350.f, 215.f });

 

  
  auto vehicle= app_commands.spawn()
    .add_component<Rectangle>(sf::Vector2f{ 625.f, 545.f })
    .add_component<Colour>(colour::white())
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<ZIndex>(2)
    .add_component<Transform>(sf::Vector2f{ -500.f, 50.f });
                                                     
  auto left_button_icon =
    app_commands.spawn()
      .add_component<Triangle>(sf::Vector2f{ 50.f, 0.f }, sf::Vector2f{ 50.f, 120.f }, sf::Vector2f{ 110.f, 60.f })
      .add_component<Colour>(colour::black())
      .add_component<Transform>(sf::Vector2f{ -805.f, 100.f });
  
  auto right_button_icon =
    app_commands.spawn()
      .add_component<Triangle>(sf::Vector2f{ 50.f, 0.f }, sf::Vector2f{ 50.f, 120.f }, sf::Vector2f{ -10.f, 60.f })
      .add_component<Colour>(colour::black())
      .add_component<Transform>(sf::Vector2f{ -95.f, 100.f });

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
    .add_component<ZIndex>(3)
    .add_component<Transform>(sf::Vector2f{ -483.f, 425.f });

  auto outline_p6 = app_commands.spawn()
    .add_component<Rectangle>(sf::Vector2f{ 56.f, 40.f })
    .add_component<Colour>(u8(255), u8(237), u8(237))
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<ZIndex>(4)
    .add_component<Transform>(sf::Vector2f{ -231.f, 425.f });

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
      .template add_component<Children>(std::vector{ prompt_1.entity(), 
                                                     prompt_2.entity(),
                                                     prompt_3.entity(), 
                                                     prompt_4.entity(),
                                                     prompt_5.entity(), 
                                                     prompt_6.entity(), 
                                                     prompt_7.entity() })
      .template add_component<layout::Margin>(layout::Margin{.top = 68.f, .left = 80.f});
  
  app_commands.spawn()
    .template add_component<layout::FlexRoot>()
    .template add_component<layout::Flex>(layout::Flex::Direction::Vertical, layout::Flex::Alignment::Start)
    .template add_component<Children>(
      std::vector{ title.entity(), 
                   subtitle.entity(),
                   select_label.entity(),
                   state_label.entity(),
                   speed_state.entity(),
                   acceleration_state.entity(),
                   fuel_state.entity(),
                   Jungle_txt.entity(),
                   left_button_icon.entity(),
                   right_button_icon.entity(),
                   bottom_row.entity(),
                   vehicle.entity(),
                   state_acceleration.entity(),
                   state_speed.entity(),
                   state_feul.entity(),
                   vehicle_speed.entity(),
                   vehicle_acceleration.entity(),
                   vehicle_feul.entity(),
                   outline_p2.entity(),
                   outline_p4.entity(),
                   outline_p6.entity()});
}

};// namespace main_menu
