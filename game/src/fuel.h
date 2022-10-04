#pragma once

#include "SFML/System/Vector2.hpp"

#include "engine/app_commands.h"
#include "engine/query.h"
#include "engine/timing.h"

#include "game/src/pause_state.h"
#include "game/src/velocity.h"

struct Fuel
{
  float burn_rate;
  float current_fuel;
  float max_capacity;
};
