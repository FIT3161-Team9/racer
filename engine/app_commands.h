#pragma once

#include "app.h"

namespace engine
{
void run_app(App const&);
};

class AppCommands
{
  entt::registry& m_registry;

  friend void engine::run_app(App const&);

  AppCommands(entt::registry& registry) : m_registry(registry) {}

public:
  Entity spawn() { return { m_registry.create(), m_registry }; }
};