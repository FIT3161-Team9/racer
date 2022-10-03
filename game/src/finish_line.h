#pragma once

#include <iostream>

#include "engine/app_commands.h"
#include "engine/query.h"

#include "engine/transform.h"
#include "game/src/pause_state.h"
#include "game/src/vehicle.h"

enum class FinishLine {};

namespace finish_line
{

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system(ResourceQuery<PauseState>{},
                          Query<FinishLine const, Transform const>{},
                          Query<Vehicle const, Transform const>{},
                          [&](auto& resources, auto& finish_line_view, auto& vehicle_view) {
                            auto&& [_, pause_state] = resources;
                            for (auto&& [finish_line_entity, _finish_line, finish_line_transform] :
                                 finish_line_view.each()) {
                              for (auto&& [vehicle_entity, vehicle, vehicle_transform] : vehicle_view.each()) {
                                if (vehicle_transform.value.x >= finish_line_transform.value.x) {
                                  pause_state.paused = true;
                                }
                              }
                            }
                          });
}

}// namespace finish_line
