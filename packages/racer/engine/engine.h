#pragma once

#include <SFML/Graphics.hpp>

#include "app.h"

namespace engine
{

App create_app(std::string_view name) { return App{ .name = name }; }

void run_app(App const& app)
{
  sf::Window window(sf::VideoMode(1920, 1080), app.name.cbegin());

  while (window.isOpen()) {

    sf::Event event;

    while (window.pollEvent(event)) {

      if (event.type == sf::Event::Closed) window.close();
    }
  }
}

}// namespace engine
