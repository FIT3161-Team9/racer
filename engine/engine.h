#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <functional>
#include <thread>

#include "app.h"
#include "app_commands.h"

namespace engine
{

namespace detail
{
  void handle_events(sf::RenderWindow&);
  void scale_view_to_window_size(sf::RenderWindow&);
}// namespace detail

std::uint64_t MS_PER_FRAME = 12;
float const COORDINATE_SPACE_WIDTH = 2000;
float const COORDINATE_SPACE_HEIGHT = 1000;
auto const ASPECT_RATIO = COORDINATE_SPACE_WIDTH / COORDINATE_SPACE_HEIGHT;

App create_app(std::string_view name) { return App{ .name = name }; }

template<typename StartupFn>
void run_app(App const& app, StartupFn on_startup)
{
  sf::RenderWindow window(sf::VideoMode(1920, 1080), app.name.data());
  sf::View view(sf::FloatRect(0, 0, COORDINATE_SPACE_WIDTH, COORDINATE_SPACE_HEIGHT));
  window.setView(view);

  auto entity_registery = entt::registry{};
  auto app_commands = AppCommands(entity_registery);

  on_startup(app_commands);

  detail::scale_view_to_window_size(window);

  sf::Clock clock;
  std::uint64_t total_time_since_render{};
  while (window.isOpen()) {
    detail::handle_events(window);
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
      // for (auto entity : app.entities) { entity->render(window); }
      window.display();
    }
  }
}

/**
 * SFML's coordinate system has (0,0) at the top left of the view.
 * We want games to have (0,0) as the center of the view. This utility function
 * transforms coordinates in the game's coordinate system to coordinates in
 * SFML's coordinate system
 */
sf::Vector2f to_screen_space(sf::Vector2f const& vec)
{
  return { vec.x + 0.5f * COORDINATE_SPACE_WIDTH, vec.y + 0.5f * COORDINATE_SPACE_HEIGHT };
}

namespace detail
{
  void scale_view_to_window_size(sf::RenderWindow& window)
  {
    auto const [width, height] = window.getSize();

    auto const scaled_height = static_cast<float>(width) / ASPECT_RATIO;

    // If the scaled height is greater than the height of the window, we want to scale
    // the width
    auto const target_width = scaled_height > height ? height * ASPECT_RATIO : static_cast<float>(width);
    auto const target_height = scaled_height > height ? height : scaled_height;

    auto const target_width_scale = target_width / width;
    auto const target_height_scale = target_height / height;

    sf::View view(sf::FloatRect(0, 0, COORDINATE_SPACE_WIDTH, COORDINATE_SPACE_HEIGHT));

    view.setViewport(
      { (1 - target_width_scale) * 0.5f, (1 - target_height_scale) * 0.5f, target_width_scale, target_height_scale });

    window.setView(view);
  }

  void handle_events(sf::RenderWindow& window)
  {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
      if (event.type == sf::Event::Resized) { detail::scale_view_to_window_size(window); }
    }
  }
}// namespace detail

}// namespace engine
