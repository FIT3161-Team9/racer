#pragma once

#include <entt/entt.hpp>
#include <vector>

#include "app.h"
#include "entity.h"
#include "event.h"
#include "query.h"

class AppCommands;

namespace engine
{
template<typename StartupFn>
void run_app(App const&, StartupFn);
namespace detail
{
  void handle_events(sf::RenderWindow& window, AppCommands& app_commands);
}
};// namespace engine

/// The interface between an app and the engine.
///
/// AppCommands allows clients of the engine to interact with the engine. It maintains
/// the list of systems and resources that the app has registered with the engine
class AppCommands
{
  /// The list of systems currently registered in the ECS.
  ///
  /// Each of these are called at each iteration of the main engine loop.
  std::vector<std::function<void()>> m_systems{};
  /// The list of event systems currently registered in the ECS. Each event system should return
  /// a boolean indicating whether the event has been handled and it's propagation should stop
  ///
  /// These are called when the specified event type triggers.
  std::vector<std::function<bool(Event)>> m_event_systems{};
  /// The registry of the Entities and their corresponding Components in the ECS.
  entt::registry& m_registry;
  /// A second ECS registry that has only the resource entity
  entt::registry m_resource_registry{};
  /// The resource entity. Each resource component hangs off of this entity in the m_resource_registry
  entt::entity m_resource_entity;

  template<typename StartupFn>
  friend void engine::run_app(App const&, StartupFn);
  friend void engine::detail::handle_events(sf::RenderWindow&, AppCommands&);

  explicit AppCommands(entt::registry& registry) : m_registry(registry)
  {
    m_resource_entity = m_resource_registry.create();
  }

  /// Run all of the currently registered systems in the ECS
  void run_systems()
  {
    for (auto system : m_systems) system();
  }

public:
  /// Spawn a new entity
  Entity spawn() { return { m_registry.create(), m_registry }; }
  Entity spawn(entt::entity hint) { return { m_registry.create(hint), m_registry }; }
  /// Destroy the given entity
  void destroy(entt::entity entity) { m_registry.destroy(entity); }
  /// Returns whether the given entity is alive or not
  bool alive(entt::entity entity) { return m_registry.valid(entity); }

  /// Add a system with a resource query and a component query
  template<typename... ResourceQueryTypes, typename... EntityQueryTypes, typename Fn>
  void add_system(ResourceQuery<ResourceQueryTypes...>, Query<EntityQueryTypes...>, Fn fn)
  {
    auto resource_tuple = *m_resource_registry.view<ResourceQueryTypes...>().each().begin();
    auto entity_view = m_registry.view<EntityQueryTypes...>();
    m_systems.push_back([=]() { fn(resource_tuple, entity_view); });
    return;
  }

  /// Add a system with a resource query
  template<typename... ResourceQueryTypes, typename Fn>
  void add_system(ResourceQuery<ResourceQueryTypes...>, Fn fn)
  {
    auto resource_tuple = *m_resource_registry.view<ResourceQueryTypes...>().each().begin();
    m_systems.push_back([=]() { fn(resource_tuple); });
    return;
  }

  /// Add a system with a resource query and two component queries
  template<typename... ResourceQueryTypes, typename... QueryOneTypes, typename... QueryTwoTypes, typename Fn>
  void add_system(ResourceQuery<ResourceQueryTypes...>, Query<QueryOneTypes...>, Query<QueryTwoTypes...>, Fn fn)
  {
    auto resource_tuple = *m_resource_registry.view<ResourceQueryTypes...>().each().begin();
    auto view_one = m_registry.view<QueryOneTypes...>();
    auto view_two = m_registry.view<QueryTwoTypes...>();
    m_systems.push_back([=]() { fn(resource_tuple, view_one, view_two); });
    return;
  }

  /// Add a system with two component queries
  template<typename... QueryOneTypes, typename... QueryTwoTypes, typename Fn>
  void add_system(Query<QueryOneTypes...>, Query<QueryTwoTypes...>, Fn fn)
  {
    auto view_one = m_registry.view<QueryOneTypes...>();
    auto view_two = m_registry.view<QueryTwoTypes...>();
    m_systems.push_back([=]() { fn(view_one, view_two); });
    return;
  }

  /// Add a system with a component query
  template<typename... QueryTypes, typename Fn>
  void add_system(Query<QueryTypes...>, Fn fn)
  {
    auto view = m_registry.view<QueryTypes...>();
    m_systems.push_back([=]() { fn(view); });
    return;
  }

  /// Add an event system
  template<Event::EventType event_type, typename Fn>
  void add_system(Fn function)
  {
    m_event_systems.push_back([=](Event event) {
      if (event.type == event_type) { return function(event); }
      return false;
    });
  }

  /// Add an event system with a component query
  template<Event::EventType event_type, typename... QueryTypes, typename Fn>
  void add_system(Query<QueryTypes...>, Fn function)
  {
    auto view = m_registry.view<QueryTypes...>();
    m_event_systems.push_back([=](Event event) {
      if (event.type == event_type) { return function(event, view); }
      return false;
    });
  }

  /// Add an event system with a resource query
  template<Event::EventType event_type, typename... ResourceQueryTypes, typename Fn>
  void add_system(ResourceQuery<ResourceQueryTypes...>, Fn function)
  {
    auto resource_tuple = *m_resource_registry.view<ResourceQueryTypes...>().each().begin();
    m_event_systems.push_back([=](Event event) {
      if (event.type == event_type) { return function(event, resource_tuple); }
      return false;
    });
  }

  /// Add an event system with a resource query and a component query
  template<Event::EventType event_type, typename... ResourceQueryTypes, typename... QueryTypes, typename Fn>
  void add_system(ResourceQuery<ResourceQueryTypes...>, Query<QueryTypes...>, Fn function)
  {
    auto resource_tuple = *m_resource_registry.view<ResourceQueryTypes...>().each().begin();
    auto view = m_registry.view<QueryTypes...>();
    m_event_systems.push_back([=](Event event) {
      if (event.type == event_type) { return function(event, resource_tuple, view); }
      return false;
    });
  }

  /// Add a plugin. Plugins are just functions that get passed a reference to AppCommands
  template<typename Plugin>
  void add_plugin(Plugin plugin)
  {
    plugin(*this);
  }

  /// Given an entity and a component, return a pointer to that component on that entity
  template<typename Component>
  Component* component(entt::entity entity)
  {
    return m_registry.try_get<Component>(entity);
  }

  /// Add a resource. A resource is a component that exists away from any entity, i.e. it is basically
  /// global state. There can only be one resource of each type. The params passed are passed to the
  /// constructor of the given resource type
  template<typename Resource, typename... ResourceArgs>
  void add_resource(ResourceArgs... args)
  {
    if (!m_resource_registry.try_get<Resource>(m_resource_entity)) {
      m_resource_registry.emplace<Resource>(m_resource_entity, std::forward<ResourceArgs>(args)...);
    }
  }

  /// Get a pointer to a resource
  template<typename Resource>
  Resource* get_resource()
  {
    return m_resource_registry.try_get<Resource>(m_resource_entity);
  }
};
