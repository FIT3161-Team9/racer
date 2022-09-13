#pragma once

#include <engine/app_commands.h>
#include <engine/circle.h>
#include <engine/rectangle.h>
#include <engine/transform.h>
#include <engine/vector_utils.h>
#include <memory>

#include "player.h"

namespace camera
{

inline void plugin(AppCommands& app_commands) {}

void updateTarget(AppCommands& app_commands)
{
  app_commands.add_system(Query<Player, Transform>{}, [](auto& view) {});
}


};// namespace camera