#pragma once

#include <memory>
#include <vector>

#include "engine/app_commands.h"
#include "engine/camera_target.h"
#include "engine/transform.h"
#include "engine/vector_utils.h"

namespace camera
{
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
  app_commands.add_system(Query<CameraTarget, Transform>{}, Query<Transform>{}, [&](auto& targets, auto& view) {
    sf::Vector2f centre_distance = {};
    for (auto&& [ent, target, transform] : targets.each()) {
      if (target.is_followed) { centre_distance = sf::Vector2f{ transform.value.x, transform.value.y }; }
    }
    for (auto&& [ent, transform] : view.each()) {
      transform.value = vector_utils::minus(transform.value, centre_distance);
    }
  });
}

/// Updates the entity that the camera is centred on
inline void update_target(AppCommands& app_commands)
{
  app_commands.add_system(Query<CameraTarget, Transform>{}, [&](auto& view) {
    std::vector<entt::entity> targets{};

    for (auto&& [ent, _target, _outline] : view.each()) { targets.push_back(ent); }

    if (targets.size() < 1) { return; }

    auto const player_one_transform = app_commands.component<Transform>(targets.front());
    auto const player_two_transform = app_commands.component<Transform>(targets.back());

    auto player_one_follow_status = app_commands.component<CameraTarget>(targets.front());
    auto player_two_follow_status = app_commands.component<CameraTarget>(targets.back());

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
