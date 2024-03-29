#pragma once

#include <engine/app_commands.h>
#include <engine/query.h>
#include <engine/timing.h>
#include <engine/transform.h>

#include "velocity.h"

namespace gravity
{

struct AffectedByGravity
{
  bool value{};
};

float constexpr GRAVITY = 9.8f;

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system(Query<Velocity, AffectedByGravity const>{}, [&](auto& view) {
    auto const elapsed_milliseconds = app_commands.get_resource<timing::ElapsedTime>()->elapsed_milliseconds;
    for (auto [entity, velocity, affected_by_gravity] : view.each()) {
      if (affected_by_gravity.value) { velocity.value.y += static_cast<float>(GRAVITY * elapsed_milliseconds * 0.01f); }
    }
  });

  app_commands.add_system(Query<Velocity const, Transform>{}, [&](auto& view) {
    auto const elapsed_milliseconds = app_commands.get_resource<timing::ElapsedTime>()->elapsed_milliseconds;
    for (auto [entity, velocity, transform] : view.each()) {
      transform.value.y += static_cast<float>(elapsed_milliseconds * velocity.value.y * 0.01f);
      transform.value.x += static_cast<float>(elapsed_milliseconds * velocity.value.x * 0.01f);
    }
  });
}

};// namespace gravity
