#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <filesystem>
#include <string>
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
#include "engine/rotation.h"
#include "engine/triangle.h"
#include "engine/zindex.h"
#include "game/src/count_down_timer.h"
#include "game/src/game_state.h"
#include "game/src/map.h"
#include "game/src/players.h"
#include "game/src/ui/background.h"
#include "game/src/ui/icon.h"
#include "game/src/utils.h"
#include "game/src/vehicle.h"

namespace vehicle_select
{

void spawn_ui(AppCommands&, std::string);
void render_selected_vehicle_stats(AppCommands&);

enum class UIElement {};
enum class VehicleInfo {};

struct SelectedVehicle
{
  // An index into the vehicles array
  int vehicle_index{};
};

struct VehicleInformation
{
  std::vector<vehicle::Information> vehicles{};
};

/// This plugin implements the game's main screen
inline void plugin(AppCommands& app_commands)
{
  app_commands.add_resource<SelectedVehicle>(0);
  app_commands.add_resource<VehicleInformation>(vehicle::load_information());
  app_commands.template add_system<Event::EventType::KeyReleased>(
    ResourceQuery<GameState, SelectedVehicle, VehicleInformation>{},
    Query<VehicleInfo>{},
    [&](auto& event, auto& resource_tuple, auto& view) {
      auto&& [_, game_state, selected_vehicle, vehicle_information] = resource_tuple;
      if (game_state.current_screen != GameState::CurrentScreen::VehicleSelectPlayerOne
          && game_state.current_screen != GameState::CurrentScreen::VehicleSelectPlayerTwo) {
        return false;
      }

      if (event.key_released.key != sf::Keyboard::Key::Right && event.key_released.key != sf::Keyboard::Key::Left) {
        return false;
      }

      if (event.key_released.key == sf::Keyboard::Key::Left) {
        selected_vehicle.vehicle_index += 1;
        if (static_cast<unsigned long>(selected_vehicle.vehicle_index) == vehicle_information.vehicles.size()) {
          selected_vehicle.vehicle_index = 0;
        }
      }

      if (event.key_released.key == sf::Keyboard::Key::Right) {
        selected_vehicle.vehicle_index -= 1;
        if (selected_vehicle.vehicle_index == -1) {
          selected_vehicle.vehicle_index = static_cast<int>(vehicle_information.vehicles.size()) - 1;
        }
      }
      // Update UI
      for (auto&& [entity, _vehicle_info] : view.each()) { app_commands.destroy(entity); }
      render_selected_vehicle_stats(app_commands);

      return false;
    });

  // Listen for the "enter" key
  app_commands.template add_system<Event::EventType::KeyReleased>(
    ResourceQuery<GameState, SelectedVehicle>{},
    Query<UIElement>{},
    [&](auto& event, auto& resource_tuple, auto& view) {
      auto&& [_, game_state, selected_vehicle] = resource_tuple;
      if (event.key_released.key != sf::Keyboard::Key::Enter) { return false; }

      if (game_state.current_screen == GameState::CurrentScreen::VehicleSelectPlayerOne) {
        game_state = game_state::player_one_select_vehicle(selected_vehicle.vehicle_index);

        for (auto&& [entity, _ui_element] : view.each()) { app_commands.destroy(entity); }
        vehicle_select::spawn_ui(app_commands, "PLAYER   TWO   SELECT   VEHICLE");

        return true;
      }
      if (game_state.current_screen == GameState::CurrentScreen::VehicleSelectPlayerTwo) {

        for (auto&& [entity, _ui_element] : view.each()) { app_commands.destroy(entity); }

        auto const level = map::LEVELS[0];
        auto const loaded_level = map::load_level(app_commands, level.c_str());

        vehicle::load(app_commands,
                      vehicle::VEHICLES[game_state.vehicle_select_player_two.player_one_vehicle].c_str(),
                      loaded_level.vehicle_spawn_location)
          .template add_component<PlayerOne>();
        vehicle::load(
          app_commands, vehicle::VEHICLES[selected_vehicle.vehicle_index].c_str(), loaded_level.vehicle_spawn_location)
          .template add_component<PlayerTwo>();

        count_down_timer::start(app_commands);

        game_state.current_screen = GameState::CurrentScreen::InLevel;

        return true;
      }
      return false;
    });
}

inline void render_selected_vehicle_stats(AppCommands& app_commands)
{
  auto const& selected_vehicle = *app_commands.get_resource<SelectedVehicle>();
  auto const& vehicle_information = *app_commands.get_resource<VehicleInformation>();
  auto const& selected_vehicle_information = vehicle_information.vehicles[selected_vehicle.vehicle_index];

  auto const speed_bar_width =
    selected_vehicle_information.speed * 700.f / vehicle::max_speed(vehicle_information.vehicles);
  auto const acceleration_bar_width =
    selected_vehicle_information.acceleration * 700.f / vehicle::max_acceleration(vehicle_information.vehicles);
  auto const fuel_bar_width =
    selected_vehicle_information.fuel * 700.f / vehicle::max_fuel(vehicle_information.vehicles);

  // speed
  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ speed_bar_width, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::white())
    .add_component<ZIndex>(4)
    .add_component<VehicleInfo>()
    .add_component<Transform>(sf::Vector2f{ 400.f - ((700.f - speed_bar_width) / 2), -80.f });

  // acceleration
  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ acceleration_bar_width, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::white())
    .add_component<ZIndex>(4)
    .add_component<VehicleInfo>()
    .add_component<Transform>(sf::Vector2f{ 400.f - ((700.f - acceleration_bar_width) / 2), 75.f });

  // fuel
  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ fuel_bar_width, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::white())
    .add_component<ZIndex>(4)
    .add_component<VehicleInfo>()
    .add_component<Transform>(sf::Vector2f{ 400.f - ((700.f - fuel_bar_width) / 2), 215.f });

  // Vehicle
  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ 625.f, 545.f })
    .add_component<Scale>(sf::Vector2f{ 1.0f, 1.0f })
    .add_component<Texture>(selected_vehicle_information.texture_path)
    .add_component<ZIndex>(4)
    .add_component<Rotation>(0.f)
    .add_component<VehicleInfo>()
    .add_component<Transform>(sf::Vector2f{ -500.f, 50.f });
}

/// Create the UI for ths main screen
inline void spawn_ui(AppCommands& app_commands, std::string str)
{
  using utils::u32;
  using utils::u8;

  auto background = background::spawn(app_commands);

  background.add_component<UIElement>();

  render_selected_vehicle_stats(app_commands);

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
                        .add_component<Text>(utils::INTER_SEMI_BOLD, str, u32(66), 0.85f)
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

  // Speed
  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ 700.f, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::black())
    .add_component<ZIndex>(3)
    .add_component<Transform>(sf::Vector2f{ 400.f, -80.f });

  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ 700.f, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::black())
    .add_component<ZIndex>(3)
    .add_component<Transform>(sf::Vector2f{ 400.f, 75.f });


  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Rectangle>(sf::Vector2f{ 700.f, 50.f })
    .add_component<Outline>(colour::black(), 2.2f)
    .add_component<Colour>(colour::black())
    .add_component<ZIndex>(3)
    .add_component<Transform>(sf::Vector2f{ 400.f, 215.f });

  // Outline
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

  // Left icon
  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Triangle>(sf::Vector2f{ 50.f, 0.f }, sf::Vector2f{ 50.f, 120.f }, sf::Vector2f{ 110.f, 60.f })
    .add_component<Colour>(colour::black())
    .add_component<ZIndex>(1)
    .add_component<Transform>(sf::Vector2f{ -805.f, 100.f });
  // Right icon
  app_commands.spawn()
    .add_component<UIElement>()
    .add_component<Triangle>(sf::Vector2f{ 50.f, 0.f }, sf::Vector2f{ 50.f, 120.f }, sf::Vector2f{ -10.f, 60.f })
    .add_component<Colour>(colour::black())
    .add_component<ZIndex>(1)
    .add_component<Transform>(sf::Vector2f{ -95.f, 100.f });

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
