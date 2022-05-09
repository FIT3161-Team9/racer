#pragma once

#include <SFML/Graphics.hpp>

/**
 * A generic entity that can be rendered to the screen
 */
class Entity
{
public:
  virtual ~Entity() = default;
  virtual void render(sf::RenderWindow&) {}
};
