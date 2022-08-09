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

class AppCommands
{
  std::vector<std::function<void()>> m_systems{};
  std::vector<std::function<void(Event)>> m_event_systems{};
  entt::registry& m_registry;
  entt::registry m_resource_registry{};
  entt::entity m_resource_entity;

  template<typename StartupFn>
  friend void engine::run_app(App const&, StartupFn);
  friend void engine::detail::handle_events(sf::RenderWindow&, AppCommands&);

  explicit AppCommands(entt::registry& registry) : m_registry(registry)
  {
    m_resource_entity = m_resource_registry.create();
  }

  void run_systems()
  {
    for (auto system : m_systems) system();
  }

public:
  Entity spawn() { return { m_registry.create(), m_registry }; }
  void destroy(entt::entity entity) { m_registry.destroy(entity); }

  template<typename... ResourceQueryTypes, typename... EntityQueryTypes, typename Fn>
  void add_system(ResourceQuery<ResourceQueryTypes...>, Query<EntityQueryTypes...>, Fn fn)
  {
    auto resource_tuple = *m_resource_registry.view<ResourceQueryTypes...>().each().begin();
    auto entity_view = m_registry.view<EntityQueryTypes...>();
    m_systems.push_back([=]() { fn(resource_tuple, entity_view); });
    return;
  }

  template<typename... ResourceQueryTypes, typename Fn>
  void add_system(ResourceQuery<ResourceQueryTypes...>, Fn fn)
  {
    auto resource_tuple = *m_resource_registry.view<ResourceQueryTypes...>().each().begin();
    m_systems.push_back([=]() { fn(resource_tuple); });
    return;
  }

  template<typename... ResourceQueryTypes, typename... QueryOneTypes, typename... QueryTwoTypes, typename Fn>
  void add_system(ResourceQuery<ResourceQueryTypes...>, Query<QueryOneTypes...>, Query<QueryTwoTypes...>, Fn fn)
  {
    auto resource_tuple = *m_resource_registry.view<ResourceQueryTypes...>().each().begin();
    auto view_one = m_registry.view<QueryOneTypes...>();
    auto view_two = m_registry.view<QueryTwoTypes...>();
    m_systems.push_back([=]() { fn(resource_tuple, view_one, view_two); });
    return;
  }

  template<typename... QueryOneTypes, typename... QueryTwoTypes, typename Fn>
  void add_system(Query<QueryOneTypes...>, Query<QueryTwoTypes...>, Fn fn)
  {
    auto view_one = m_registry.view<QueryOneTypes...>();
    auto view_two = m_registry.view<QueryTwoTypes...>();
    m_systems.push_back([=]() { fn(view_one, view_two); });
    return;
  }

  template<typename... QueryTypes, typename Fn>
  void add_system(Query<QueryTypes...>, Fn fn)
  {
    auto view = m_registry.view<QueryTypes...>();
    m_systems.push_back([=]() { fn(view); });
    return;
  }

  template<Event::EventType event_type, typename... QueryTypes, typename Fn>
  void add_system(Query<QueryTypes...>, Fn function)
  {
    auto view = m_registry.view<QueryTypes...>();
    m_event_systems.push_back([=](Event event) {
      if (event.type == event_type) function(event, view);
    });
  }

  template<Event::EventType event_type, typename... QueryTypes, typename Fn>
  void add_system(ResourceQuery<QueryTypes...>, Fn function)
  {
    auto resource_tuple = *m_resource_registry.view<QueryTypes...>().each().begin();
    m_event_systems.push_back([=](Event event) {
      if (event.type == event_type) function(event, resource_tuple);
    });
  }

  template<Event::EventType event_type, typename... ResourceQueryTypes, typename... QueryTypes, typename Fn>
  void add_system(ResourceQuery<ResourceQueryTypes...>, Query<QueryTypes...>, Fn function)
  {
    auto resource_tuple = *m_resource_registry.view<ResourceQueryTypes...>().each().begin();
    auto view = m_registry.view<QueryTypes...>();
    m_event_systems.push_back([=](Event event) {
      if (event.type == event_type) function(event, resource_tuple, view);
    });
  }


  template<typename Plugin>
  void add_plugin(Plugin plugin)
  {
    plugin(*this);
  }

  template<typename Component>
  Component* component(entt::entity entity)
  {
    return m_registry.try_get<Component>(entity);
  }

  template<typename Resource, typename... ResourceArgs>
  void add_resource(ResourceArgs... args)
  {
    if (!m_resource_registry.try_get<Resource>(m_resource_entity)) {
      m_resource_registry.emplace<Resource>(m_resource_entity, std::forward<ResourceArgs>(args)...);
    }
  }

  template<typename Resource>
  Resource* get_resource()
  {
    return m_resource_registry.try_get<Resource>(m_resource_entity);
  }
};
