#pragma once

#include <SFML/Graphics.hpp>

#include "app.h"

namespace engine
{

float const COORDINATE_SPACE_WIDTH = 200;
float const COORDINATE_SPACE_HEIGHT = 200;

App create_app(std::string_view name) { return App{ .name = name }; }

void run_app(App const& app)
{
  sf::Window window(sf::VideoMode(1920, 1080), app.name.cbegin());

  while (window.isOpen()) {
    window.clear();

    sf::Event event;

    while (window.pollEvent(event)) {

      if (event.type == sf::Event::Closed) window.close();
    }

    for (auto entity : app.entities) { entity->render(window); }

    window.display();
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

template<typename EntityType, typename... EntityArgs>
std::shared_ptr<EntityType> spawn_entity(App& app, EntityArgs... args)
{
  auto entity = std::make_shared<EntityType>(std::forward<EntityArgs>(args)...);

  app.entities.push_back(entity);

  return entity;
}

}// namespace engine
