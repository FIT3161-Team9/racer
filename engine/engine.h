#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <functional>
#include <thread>

#include "app.h"
#include "app_commands.h"
#include "event.h"
#include "window.h"

namespace engine
{

namespace detail
{
  template<typename AppState>
  void handle_events(sf::RenderWindow&, AppCommands<AppState>&);
}// namespace detail

std::uint64_t MS_PER_FRAME = 12;

App create_app(std::string_view name) { return App{ .name = name }; }

template<typename StateType, typename StartupFn>
void run_app(App const& app, StartupFn on_startup)
{
  sf::RenderWindow window(sf::VideoMode(1920, 1080), app.name.data());
  sf::View view(sf::FloatRect(0, 0, window::COORDINATE_SPACE_WIDTH, window::COORDINATE_SPACE_HEIGHT));
  window.setView(view);

  auto entity_registery = entt::registry{};
  auto app_commands = AppCommands<StateType>(entity_registery);

  on_startup(app_commands);

  window::scale_view_to_window_size(window);

  sf::Clock clock;
  std::uint64_t total_time_since_render{};
  while (window.isOpen()) {
    detail::handle_events(window, app_commands);
    sf::Time const elapsed = clock.restart();
    std::uint64_t const elapsed_milliseconds = elapsed.asMilliseconds();

    // If we are updating too quickly, slow down (otherwise no time will ever pass in the system)
    if (elapsed_milliseconds < 1) {
      using namespace std::this_thread;// sleep_for, sleep_until
      using namespace std::chrono;// nanoseconds, system_clock, seconds

      sleep_for(milliseconds(1));
    }

    total_time_since_render += elapsed_milliseconds;

    app_commands.run_systems();

    if (total_time_since_render > MS_PER_FRAME) {
      total_time_since_render -= MS_PER_FRAME;
      window.clear();
      window.display();
    }
  }
}

namespace detail
{
  template<typename AppState>
  void handle_events(sf::RenderWindow& window, AppCommands<AppState>& app_commands)
  {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
      if (event.type == sf::Event::Resized) { window::scale_view_to_window_size(window); }
      if (event.type == sf::Event::MouseMoved) {
        auto mouse_moved_event = event::mouse_moved(
          window::from_screen_space(window.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y })));
        for (auto event_handler : app_commands.m_event_systems) event_handler(mouse_moved_event);
      };
      if (event.type == sf::Event::MouseButtonPressed) {
        auto mouse_button_event = event::mouse_button_pressed(
          window::from_screen_space(window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y })));
        for (auto event_handler : app_commands.m_event_systems) event_handler(mouse_button_event);
      }
      if (event.type == sf::Event::MouseButtonReleased) {
        auto mouse_button_event = event::mouse_button_released(
          window::from_screen_space(window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y })));
        for (auto event_handler : app_commands.m_event_systems) event_handler(mouse_button_event);
      }
    }
  }
}// namespace detail

}// namespace engine
