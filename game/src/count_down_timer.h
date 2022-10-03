#pragma once

#include <optional>
#include <string>

#include "SFML/System/Clock.hpp"
#include "SFML/System/Vector2.hpp"
#include <entt/entt.hpp>

#include "engine/app_commands.h"
#include "engine/colour.h"
#include "engine/query.h"
#include "engine/text.h"
#include "engine/transform.h"

#include "engine/zindex.h"
#include "game/src/pause_state.h"
#include "game/src/utils.h"

struct CountDownTimer
{
  std::optional<sf::Clock> timer{};
};

enum class CountDownText {};

namespace count_down_timer
{

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_resource<CountDownTimer>();

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
          app_commands.destroy(entity);
        }
      }
    });
}

inline entt::entity start(AppCommands& app_commands)
{
  auto* timer = app_commands.get_resource<CountDownTimer>();

  timer->timer = sf::Clock();

  timer->timer.value().restart();

  return app_commands.spawn()
    .add_component<Transform>(sf::Vector2f{ 0.f, 0.f })
    .add_component<Colour>(colour::black())
    .add_component<ZIndex>(4)
    .add_component<Text>(utils::INTER_SEMI_BOLD, std::string{ "3" }, utils::u32(70), 1.f)
    .add_component<CountDownText>()
    .entity();
}

}// namespace count_down_timer
