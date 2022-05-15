#pragma once

#include <entt/entt.hpp>

class Entity
{
  friend class AppCommands;
  entt::entity m_entity;
  entt::registry& m_registry;

  Entity(entt::entity entity, entt::registry& registry) : m_entity(entity), m_registry(registry) {}

public:
  template<typename T, typename... TArgs>
  Entity& add_component(TArgs... args)
  {
    m_registry.emplace<T>(m_entity, std::forward<TArgs>(args)...);
    return *this;
  }
};
