#pragma once

#include <SFML/Graphics.hpp>

#include "engine.h"
#include "entity.h"

class Circle : public Entity
{
  sf::Vector2f m_center;
  float m_radius;

  // The top right coordinate of this circle
  // We need this because sfml renders circles relative to their top right
  sf::Vector2f top_left() { return { m_center.x - m_radius, m_center.y - m_radius }; }

public:
  Circle(sf::Vector2f position, float radius) : m_center(position), m_radius(radius) {}

  virtual void render(sf::RenderWindow& window) override
  {
    sf::CircleShape shape(m_radius);

    shape.setFillColor(sf::Color(100, 250, 50));

    shape.setPosition(engine::to_screen_space(top_left()));

    window.draw(shape);
  }
};
