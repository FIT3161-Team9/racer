#include <SFML/Graphics.hpp>
#include <cmath>
#include <entt/entt.hpp>
#include <functional>
#include <iostream>
#include <optional>
#include <string_view>

#include "engine/app_commands.h"
#include "engine/circle.h"
#include "engine/engine.h"
#include "engine/transform.h"

using AppState = std::optional<float>;

int main()
{
  auto app = engine::create_app("RACER");
  engine::run_app<AppState>(app, [](AppCommands<AppState>& app_commands) -> AppState {
    app_commands.spawn().add_component<Transform>(sf::Vector2f{ 1.f, 0.f }).add_component<Circle>(80.f);

    app_commands.spawn().add_component<Transform>(sf::Vector2f{ 0.f, 0.f }).add_component<Circle>(80.f);

    app_commands.add_system<Transform>(
      [&](auto& view) { view.each([&](auto& transform) { app_commands.set_state(transform.value.x); }); });

    app_commands.on_state_change([](auto before, auto after) {
      if (before) std::cout << "Before: " << static_cast<int>(*before) << '\n';
      if (after) std::cout << "After: " << static_cast<int>(*after) << '\n';
    });

    return {};
  });
  return 0;
}
