#pragma once

#include <optional>
#include <string>

#include "SFML/System/Clock.hpp"
#include "SFML/System/Vector2.hpp"
#include <entt/entt.hpp>
#include <vector>

#include "engine/app_commands.h"
#include "engine/children.h"
#include "engine/colour.h"
#include "engine/layout.h"
#include "engine/query.h"
#include "engine/text.h"
#include "engine/transform.h"
#include "engine/zindex.h"

#include "game/src/cross_line.h"
#include "game/src/pause_state.h"
#include "game/src/utils.h"

struct CountDownTimer
{
  std::optional<sf::Clock> timer{};
};

enum class CountDownText {};

namespace count_down_timer
{

enum class UIElement {};

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_resource<CountDownTimer>();

  /// Update the countdown number when the timer exists
  /// and delete the timer when it reaches 3 seconds
  app_commands.add_system(
    ResourceQuery<CountDownTimer, PauseState>{}, Query<CountDownText, Text>{}, [&](auto& resources, auto& view) {
      auto&& [_, count_down_timer, pause_state] = resources;

      if (!count_down_timer.timer.has_value()) { return; }

      for (auto&& [entity, count_down_text, text] : view.each()) {
        auto const elapsed_seconds = count_down_timer.timer.value().getElapsedTime().asSeconds();
        if (elapsed_seconds > 1.f) { text.content = "2"; }
        if (elapsed_seconds > 2.f) { text.content = "1"; }
        if (elapsed_seconds > 3.f) {
          pause_state.paused = false;
          count_down_timer.timer.reset();
          cross_line::start_timer(app_commands);
        }
      }
    });

  /// Delete the UI if the timer no longer exists
  app_commands.add_system(ResourceQuery<CountDownTimer>{}, Query<UIElement>{}, [&](auto& resources, auto& view) {
    auto&& [_, count_down_timer] = resources;

    if (count_down_timer.timer.has_value()) { return; }

    for (auto&& [entity, _ui_element] : view.each()) { app_commands.destroy(entity); }
  });
}

inline entt::entity start(AppCommands& app_commands)
{
  auto* timer = app_commands.get_resource<CountDownTimer>();

  timer->timer = sf::Clock();

  timer->timer.value().restart();

  auto const starting_in_text =
    app_commands.spawn()
      .add_component<UIElement>()
      .add_component<Colour>(colour::black())
      .add_component<layout::Margin>(0.f, 0.f, 45.f, 0.f)
      .add_component<Text>(utils::INTER_SEMI_BOLD, std::string{ "RACING IN..." }, utils::u32(55), 2.0f)
      .entity();

  auto const number_text = app_commands.spawn()
                             .add_component<UIElement>()
                             .add_component<Colour>(colour::black())
                             .add_component<Text>(utils::INTER_SEMI_BOLD, std::string{ "3" }, utils::u32(155), 1.f)
                             .add_component<CountDownText>()
                             .entity();

  return app_commands.spawn()
    .add_component<UIElement>()
    .add_component<layout::Flex>(layout::Flex::Direction::Vertical, layout::Flex::Alignment::Center)
    .add_component<layout::FlexRoot>()
    .add_component<layout::Margin>(layout::margin_top(380.f))
    .add_component<Children>(std::vector{ starting_in_text, number_text })
    .entity();
}

}// namespace count_down_timer
