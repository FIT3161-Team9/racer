#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "entity.h"

class AppCommands;

struct App
{
  std::string_view const name;
  std::function<void(AppCommands)> on_startup;
};
