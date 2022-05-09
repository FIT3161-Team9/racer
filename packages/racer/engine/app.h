#pragma once

#include <SFML/Graphics.hpp>

namespace engine
{

void run_app()
{
  sf::Window window(sf::VideoMode(800, 600), "FIT3161 Team 9 Engine");

  while (window.isOpen()) {

    sf::Event event;

    while (window.pollEvent(event)) {

      if (event.type == sf::Event::Closed) window.close();
    }
  }
}

}// namespace engine
