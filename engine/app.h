#pragma once

#include <string_view>

class AppCommands;

struct App
{
  std::string_view const name;
  std::function<void(AppCommands)> on_startup;
};
