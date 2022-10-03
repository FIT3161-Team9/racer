#pragma once

#include <optional>

#include <SFML/Graphics.hpp>

#include "engine/app_commands.h"

namespace cross_line
{

struct Timer
{
  std::optional<sf::Clock> value{};
};

void start_timer(AppCommands&);

void plugin(AppCommands&);

}// namespace cross_line
