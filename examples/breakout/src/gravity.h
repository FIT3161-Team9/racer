#pragma once

#include "../../../engine/app_commands.h"
#include "../../../engine/timing.h"
#include "../../../engine/transform.h"

#include "velocity.h"

namespace gravity
{

struct AffectedByGravity
{
  bool value{};
};

float const GRAVITY = 9.8;

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system<Velocity, AffectedByGravity const>([&](auto& view) {
    auto const elapsed_milliseconds = app_commands.get_resource<timing::ElapsedTime>()->elapsed_milliseconds;
    for (auto [entity, velocity, affected_by_gravity] : view.each()) {
      if (affected_by_gravity.value) { velocity.value.y += GRAVITY * elapsed_milliseconds * 0.01; }
    }
  });

  app_commands.add_system<Velocity const, Transform>([&](auto& view) {
    auto const elapsed_milliseconds = app_commands.get_resource<timing::ElapsedTime>()->elapsed_milliseconds;
    for (auto [entity, velocity, transform] : view.each()) {
      transform.value.y += elapsed_milliseconds * velocity.value.y * 0.01;
      transform.value.x += elapsed_milliseconds * velocity.value.x * 0.01;
    }
  });
}

};// namespace gravity
