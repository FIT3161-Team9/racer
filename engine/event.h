#pragma once

#include <SFML/Graphics.hpp>

struct Event
{
  // Type Definitions
  enum class EventType { MouseMoved, MouseButtonPressed, MouseButtonReleased };
  struct MouseMoved
  {
    sf::Vector2f location;
  };

  struct MouseButtonPressed
  {
    sf::Vector2f location;
  };

  struct MouseButtonReleased
  {
    sf::Vector2f location;
  };

  // Members
  EventType type;
  union {
    MouseMoved mouse_moved;
    MouseButtonPressed mouse_button_pressed;
    MouseButtonReleased mouse_button_released;
  };
};

// Constuction Helpers
namespace event
{

Event mouse_moved(sf::Vector2f location)
{
  return Event{ .type = Event::EventType::MouseMoved, .mouse_moved = { .location = location } };
}

Event mouse_button_pressed(sf::Vector2f location)
{
  return Event{ .type = Event::EventType::MouseButtonPressed, .mouse_button_pressed = { .location = location } };
}

Event mouse_button_released(sf::Vector2f location)
{
  return Event{ .type = Event::EventType::MouseButtonReleased, .mouse_button_released = { .location = location } };
}

}// namespace event
