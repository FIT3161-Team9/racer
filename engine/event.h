#pragma once

#include <SFML/Graphics.hpp>

struct Event
{
  // Type Definitions
  enum class EventType { MouseMoved, MouseButtonPressed, MouseButtonReleased, KeyPressed, KeyReleased };
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

  struct KeyPressed
  {
    sf::Keyboard::Key key;
  };

  struct KeyReleased
  {
    sf::Keyboard::Key key;
  };

  // Members
  EventType type;
  union {
    MouseMoved mouse_moved;
    MouseButtonPressed mouse_button_pressed;
    MouseButtonReleased mouse_button_released;
    KeyPressed key_pressed;
    KeyReleased key_released;
  };
};

// Constuction Helpers
namespace event
{

inline Event mouse_moved(sf::Vector2f location)
{
  return Event{ .type = Event::EventType::MouseMoved, .mouse_moved = { .location = location } };
}

inline Event mouse_button_pressed(sf::Vector2f location)
{
  return Event{ .type = Event::EventType::MouseButtonPressed, .mouse_button_pressed = { .location = location } };
}

inline Event mouse_button_released(sf::Vector2f location)
{
  return Event{ .type = Event::EventType::MouseButtonReleased, .mouse_button_released = { .location = location } };
}

inline Event key_pressed(sf::Keyboard::Key key)
{
  return Event{ .type = Event::EventType::KeyPressed, .key_pressed = { .key = key } };
}

inline Event key_released(sf::Keyboard::Key key)
{
  return Event{ .type = Event::EventType::KeyReleased, .key_released = { .key = key } };
}

}// namespace event
