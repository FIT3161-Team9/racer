#pragma once

#include "app.h"

namespace engine
{
void run_app(App const&);
};

class AppCommands
{
  friend void engine::run_app(App const&);

  AppCommands() {}
};