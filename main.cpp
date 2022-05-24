#include <SFML/Graphics.hpp>
#include <cmath>
#include <entt/entt.hpp>
#include <functional>
#include <iostream>
#include <optional>
#include <string_view>

#include "engine/app_commands.h"
#include "engine/circle.h"
#include "engine/colour.h"
#include "engine/engine.h"
#include "engine/event.h"
#include "engine/transform.h"

#include "src/app_state.h"
#include "src/drag_and_drop_circles.h"

int main()
{
  auto app = engine::create_app("RACER");
  engine::run_app<AppState>(app, [](AppCommands<AppState>& app_commands) -> AppState {
    app_commands.add_plugin(drag_and_drop_circles::plugin);

    app_commands.spawn()
      .add_component<Transform>(sf::Vector2f{ 0.f, 0.f })
      .add_component<Circle>(80.f)
      .add_component<Colour>(colour::alpha(colour::red(), 150))
      .add_component<drag_and_drop_circles::Draggable>();

    app_commands.spawn()
      .add_component<Transform>(sf::Vector2f{ 0.f, 0.f })
      .add_component<Circle>(80.f)
      .add_component<Colour>(colour::alpha(colour::blue(), 150))
      .add_component<drag_and_drop_circles::Draggable>();

    app_commands.on_state_change([](auto before, auto after) {
      if (before) std::cout << "Before: " << static_cast<int>(*before) << '\n';
      if (after) std::cout << "After: " << static_cast<int>(*after) << '\n';
    });

    app_commands.add_system<Event::EventType::MouseButtonPressed, Transform const>([](auto event, auto& view) {
      view.each([&](auto const& transform) {
        std::cout << "Mouse down at " << event.mouse_button_pressed.location.x
                  << " checking for hit against: " << transform.value.x << '\n';
      });
    });

    return {};
  });
  return 0;
}
