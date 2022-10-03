#pragma once

#include "engine/app_commands.h"
#include "engine/event.h"
#include "engine/transform.h"

#include "game/src/camera.h"
#include "game/src/pause_state.h"

namespace scroll_camera
{

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_system<Event::EventType::MouseWheelScrolled>(Query<Transform>{}, [&](auto& event, auto& view) {
    if (app_commands.get_resource<PauseState>() == nullptr) { return false; }
    view.each([&](auto entity, auto& transform) {
      if (app_commands.component<camera::Sticky>(entity) != nullptr) { return; }
      transform.value.x += event.mouse_wheel_scrolled.distance.x * 1.1f;
      transform.value.y += event.mouse_wheel_scrolled.distance.y * 1.1f;
    });
    return false;
  });
}

}// namespace scroll_camera
