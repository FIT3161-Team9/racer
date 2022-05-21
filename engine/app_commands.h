#pragma once

#include <entt/entt.hpp>
#include <vector>

#include "app.h"
#include "entity.h"
#include "event.h"

template<typename StateType>
class AppCommands;

namespace engine
{
template<typename AppState, typename StartupFn>
void run_app(App const&, StartupFn);
namespace detail
{
  template<typename AppState>
  void handle_events(sf::RenderWindow& window, AppCommands<AppState>& app_commands);
}
};// namespace engine

template<typename AppStateType>
class AppCommands
{
  std::vector<std::function<void()>> m_systems{};
  std::vector<std::function<void(Event)>> m_event_systems{};
  std::vector<std::function<void(AppStateType, AppStateType)>> m_state_change_listeners{};
  entt::registry& m_registry;
  AppStateType m_state;
  entt::dispatcher m_dispatcher_state_change_event{};

  struct StateChangeEvent
  {
    AppStateType previous_state;
    AppStateType new_state;
  };

  template<typename AppState, typename StartupFn>
  friend void engine::run_app(App const&, StartupFn);
  template<typename AppState>
  friend void engine::detail::handle_events(sf::RenderWindow&, AppCommands<AppState>&);

  void notify_listeners_of_state_change(StateChangeEvent& change)
  {
    for (auto listener : m_state_change_listeners) { listener(change.previous_state, change.new_state); }
  }

  AppCommands(entt::registry& registry) : m_registry(registry)
  {
    m_dispatcher_state_change_event.sink<StateChangeEvent>()
      .template connect<&AppCommands<AppStateType>::notify_listeners_of_state_change>(*this);
  }

  void run_systems()
  {
    for (auto system : m_systems) system();
  }

public:
  Entity spawn() { return { m_registry.create(), m_registry }; }

  template<Event::EventType event_type, typename... ComponentTypes, typename Fn>
  void add_system(Fn function)
  {
    m_event_systems.push_back([=](Event event) {
      if constexpr (sizeof...(ComponentTypes)) {
        auto view = m_registry.view<ComponentTypes...>();
        if (event.type == event_type) function(event, view);
      } else {
        if (event.type == event_type) function(event);
      }
    });
  }

  template<typename... ComponentTypes, typename Fn>
  void add_system(Fn function)
  {
    m_systems.push_back([=]() {
      auto view = m_registry.view<ComponentTypes...>();
      function(view);
    });
  }

  template<typename Fn>
  void on_state_change(Fn listener)
  {
    m_state_change_listeners.push_back(listener);
  }

  void set_state(AppStateType value)
  {
    // Shortcircuit if the value hasn't changed
    if (value == m_state) return;
    m_dispatcher_state_change_event.trigger(StateChangeEvent{ .previous_state = m_state, .new_state = value });
    m_state = value;
  }

  AppStateType current_state() { return m_state; }
};
