#pragma once

#include <entt/entt.hpp>

/// A thin wrapper around an entt entity that provides a nice API for adding components
class Entity
{
  friend class AppCommands;
  /// The entt entity that this entity represents
  entt::entity m_entity;
  /// A reference to the ECS registry. We need this to provide the nice API for adding components
  entt::registry& m_registry;

  Entity(entt::entity entity, entt::registry& registry) : m_entity(entity), m_registry(registry) {}

public:
  /// Convenient and chainable API for adding components to the entity
  template<typename T, typename... TArgs>
  Entity& add_component(TArgs... args)
  {
    m_registry.emplace<T>(m_entity, std::forward<TArgs>(args)...);
    return *this;
  }

  /// Get the underlying entt entity
  entt::entity entity() const { return m_entity; }
};
