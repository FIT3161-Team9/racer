#pragma once

#include <vector>

#include "app.h"

namespace engine
{
void run_app(App const&);
};

class AppCommands
{
  std::vector<std::function<void()>> m_systems{};
  entt::registry& m_registry;

  friend void engine::run_app(App const&);

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