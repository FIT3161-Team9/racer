#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "engine/app_commands.h"
#include "engine/transform.h"
#include "engine/vector_utils.h"
#include "engine/window.h"

#include "./debug/pausable.h"

namespace camera
{

struct Target
{
  bool is_followed;
};

/// Entities with this component won't have their transform updated in response to camera movement
enum class Sticky {};


inline void update_positions(AppCommands& app_commands);
inline void update_target(AppCommands& app_commands);

inline void plugin(AppCommands& app_commands)
{
  update_target(app_commands);
  update_positions(app_commands);
}

/// Moves all entities relative to the centre of the camera
inline void update_positions(AppCommands& app_commands)
{
  app_commands.add_system(Query<Target, Transform>{}, Query<Transform>{}, [&](auto& targets, auto& view) {
    sf::Vector2f centre_distance = {};
    // if (app_commands.get_resource<debug::pausable::PauseState>() != nullptr) { return; }
    for (auto&& [ent, target, transform] : targets.each()) {
      if (target.is_followed) {
        centre_distance = sf::Vector2f{ transform.value.x, transform.value.y - 0.2f * window::COORDINATE_SPACE_HEIGHT };
      }
    }
    if (centre_distance.x < 0.f && centre_distance.y < 0.f) { return; }
    for (auto&& [entity, transform] : view.each()) {
      if (app_commands.component<Sticky>(entity) != nullptr) { continue; }

      if (centre_distance.y > 0.f) { transform.value.y = transform.value.y - centre_distance.y; }
      if (centre_distance.x > 0.f) { transform.value.x = transform.value.x - centre_distance.x; }
    }
  });

  app_commands.add_system<Event::EventType::MouseWheelScrolled>(Query<Transform>{}, [&](auto& event, auto& view) {
    if (app_commands.get_resource<debug::pausable::PauseState>() == nullptr) { return false; }
    view.each([&](auto entity, auto& transform) {
      if (app_commands.component<Sticky>(entity) != nullptr) { return; }
      transform.value.x += event.mouse_wheel_scrolled.distance.x * 1.1f;
      transform.value.y += event.mouse_wheel_scrolled.distance.y * 1.1f;
    });
    return false;
  });
}

/// Updates the entity that the camera is centred on
inline void update_target(AppCommands& app_commands)
{
  app_commands.add_system(Query<Target, Transform>{}, [&](auto& view) {
    std::vector<entt::entity> targets{};

    for (auto&& [ent, _target, _outline] : view.each()) { targets.push_back(ent); }

    if (targets.size() < 1) { return; }

    auto const player_one_transform = app_commands.component<Transform>(targets.front());
    auto const player_two_transform = app_commands.component<Transform>(targets.back());

    auto player_one_follow_status = app_commands.component<Target>(targets.front());
    auto player_two_follow_status = app_commands.component<Target>(targets.back());

    if (player_one_transform->value.x > player_two_transform->value.x) {
      player_one_follow_status->is_followed = true;
      player_two_follow_status->is_followed = false;
    } else {
      player_one_follow_status->is_followed = false;
      player_two_follow_status->is_followed = true;
    }
  });
}

};// namespace camera
