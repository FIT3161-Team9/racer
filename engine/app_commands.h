#pragma once

#include <entt/entt.hpp>
#include <vector>

#include "app.h"
#include "entity.h"

class AppCommands;

namespace engine
{
template<typename StartupFn>
void run_app(App const& app, StartupFn on_startup);
};

class AppCommands
{
  std::vector<std::function<void()>> m_systems{};
  entt::registry& m_registry;

  template<typename StartupFn>
  friend void engine::run_app(App const& app, StartupFn on_startup);

  AppCommands(entt::registry& registry) : m_registry(registry) {}

  void run_systems()
  {
    for (auto system : m_systems) system();
  }

public:
  Entity spawn() { return { m_registry.create(), m_registry }; }

  template<typename... ComponentTypes, typename Fn>
  void add_system(Fn function)
  {
    m_systems.push_back([=]() {
      auto view = m_registry.view<ComponentTypes...>();
      function(view);
    });
  }
};