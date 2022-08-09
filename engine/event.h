#pragma once

#include <SFML/Graphics.hpp>

/// The events that apps can subscribe to listen to. These map to corresponding SFML events
struct Event
{
  /// Enum representing the list of events that we allow apps to subscribe to
  enum class EventType { MouseMoved, MouseButtonPressed, MouseButtonReleased, KeyPressed, KeyReleased };
  /// Mouse moved event with the location that it moved to. The location is in the engine's coordinate system
  struct MouseMoved
  {
    sf::Vector2f location;
  };

  /// Mouse button pressed down event with the location that it was clicked at. The location is in the engine's
  /// coordinate system
  struct MouseButtonPressed
  {
    sf::Vector2f location;
  };

  /// Mouse button released event with the location that it was clicked at. The location is in the engine's coordinate
  /// system
  struct MouseButtonReleased
  {
    sf::Vector2f location;
  };

  /// Keyboard key pressed event
  struct KeyPressed
  {
    sf::Keyboard::Key key;
  };

  /// Keyboard key released event
  struct KeyReleased
  {
    sf::Keyboard::Key key;
  };

  /// The type of event
  EventType type;

  /// The data for this event
  union {
    MouseMoved mouse_moved;
    MouseButtonPressed mouse_button_pressed;
    MouseButtonReleased mouse_button_released;
    KeyPressed key_pressed;
    KeyReleased key_released;
  };
};

// Constuction Helpers - thin wrappers around event constructor
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
