#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <entt/entt.hpp>
#include <functional>
#include <thread>

#include "app.h"
#include "app_commands.h"
#include "event.h"
#include "render.h"
#include "render_context.h"
#include "window.h"

namespace engine
{

namespace detail
{
  void handle_events(sf::RenderWindow&, AppCommands&);
}// namespace detail

constexpr std::uint64_t MS_PER_FRAME = 12;

/// Helper function for creating a new app
inline App create_app(std::string_view name) { return App{ .name = name }; }

/// Run the app with the given name. The StartupFn get's called after the Window and ECS
/// have been initialised, and gets given a reference to the AppCommands instance in order
/// to spawn entities, add components and add systems. This function doesn't return until
/// the user closes the app window. It runs the main app loop, renders graphics to the screen,
/// handles events and runs the ECS systems that the app has registered
template<typename StartupFn>
void run_app(App const& app, StartupFn on_startup)
{
  // Initialise the window and setup the coordinate system
  sf::RenderWindow window(sf::VideoMode(1920, 1080), app.name.data());
  sf::View view(sf::FloatRect(0, 0, window::COORDINATE_SPACE_WIDTH, window::COORDINATE_SPACE_HEIGHT));
  window.setView(view);

  // Create the ECS registry, render context and AppCommands instance
  auto entity_registery = entt::registry{};
  auto render_context = RenderContext{ entity_registery };
  auto app_commands = AppCommands(entity_registery);

  // Call the passed in callback
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
      using namespace std::this_thread;
      using namespace std::chrono;

      sleep_for(milliseconds(1));
    }

    total_time_since_render += elapsed_milliseconds;

    app_commands.run_systems();

    if (total_time_since_render > MS_PER_FRAME) {
      total_time_since_render -= MS_PER_FRAME;
      window.clear();
      render::update_view(render_context, window, entity_registery);
      render::all(render_context, window, entity_registery);
      window.display();
    }
  }
}

namespace detail
{
  inline void handle_events(sf::RenderWindow& window, AppCommands& app_commands)
  {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
      if (event.type == sf::Event::Resized) { window::scale_view_to_window_size(window); }
      if (event.type == sf::Event::MouseMoved) {
        auto mouse_moved_event = event::mouse_moved(
          window::from_screen_space(window.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y })));
        for (auto event_handler : app_commands.m_event_systems) {
          if (event_handler(mouse_moved_event)) { break; }
        }
      };
      if (event.type == sf::Event::MouseButtonPressed) {
        auto mouse_button_event = event::mouse_button_pressed(
          window::from_screen_space(window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y })));
        for (auto event_handler : app_commands.m_event_systems) {
          if (event_handler(mouse_button_event)) { break; }
        }
      }
      if (event.type == sf::Event::MouseButtonReleased) {
        auto mouse_button_event = event::mouse_button_released(
          window::from_screen_space(window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y })));
        for (auto event_handler : app_commands.m_event_systems) {
          if (event_handler(mouse_button_event)) { break; }
        }
      }
      if (event.type == sf::Event::KeyPressed) {
        auto key_pressed_event = event::key_pressed(event.key.code);
        for (auto event_handler : app_commands.m_event_systems) {
          if (event_handler(key_pressed_event)) { break; }
        }
      }
      if (event.type == sf::Event::KeyReleased) {
        auto key_released_event = event::key_released(event.key.code);
        for (auto event_handler : app_commands.m_event_systems) {
          if (event_handler(key_released_event)) { break; }
        }
      }
    }
  }
}// namespace detail

}// namespace engine
