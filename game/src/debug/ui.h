#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "engine/app_commands.h"
#include "engine/children.h"
#include "engine/colour.h"
#include "engine/layout.h"
#include "engine/rectangle.h"
#include "engine/text.h"
#include "engine/transform.h"
#include "engine/vector_utils.h"
#include "engine/window.h"
#include "engine/zindex.h"

#include "game/src/utils.h"

namespace debug::ui
{

entt::entity spawn_title(AppCommands&);
entt::entity spawn_entity_information(AppCommands&);
entt::entity spawn_entity_information_title(AppCommands&);
entt::entity spawn_entity_transform_information(AppCommands&);
entt::entity spawn_entity_size_information(AppCommands&);

inline void spawn_menu(AppCommands& app_commands)
{
  // Menu background
  app_commands.spawn()
    .add_component<Rectangle>(sf::Vector2f{ 400.f, window::COORDINATE_SPACE_HEIGHT })
    .add_component<Transform>(sf::Vector2f{ window::COORDINATE_SPACE_WIDTH * 0.5f - 200.f, 0.f })
    .add_component<ZIndex>(3)
    .add_component<Colour>(Colour{ .r = 77, .g = 77, .b = 77 })
    .add_component<layout::FlexRoot>()
    .add_component<layout::Flex>(layout::Flex::Direction::Vertical, layout::Flex::Alignment::Start)
    .add_component<Children>(std::vector{ spawn_title(app_commands), spawn_entity_information(app_commands) });

  // Edge border
  app_commands.spawn()
    .add_component<Rectangle>(sf::Vector2f{ 2.f, window::COORDINATE_SPACE_HEIGHT })
    .add_component<Transform>(sf::Vector2f{ window::COORDINATE_SPACE_WIDTH * 0.5f - 398.f, 0.f })
    .add_component<ZIndex>(4)
    .add_component<Colour>(Colour{ .r = 107, .g = 104, .b = 107 });
}

inline entt::entity spawn_title(AppCommands& app_commands)
{
  using utils::u32;
  return app_commands.spawn()
    .add_component<Text>(utils::INTER_SEMI_BOLD, "Debug Menu", u32(20), 1.f)
    .add_component<Colour>(colour::white())
    .add_component<layout::Margin>(10.f, 0.f, 0.f, 10.f)
    .entity();
}

inline entt::entity spawn_entity_information(AppCommands& app_commands)
{
  return app_commands.spawn()
    .add_component<layout::Flex>(layout::Flex::Direction::Vertical, layout::Flex::Alignment::Start)
    .add_component<Children>(std::vector{ spawn_entity_information_title(app_commands),
                                          spawn_entity_transform_information(app_commands),
                                          spawn_entity_size_information(app_commands) })
    .entity();
}

inline entt::entity spawn_entity_information_title(AppCommands& app_commands)
{
  using utils::u32;
  return app_commands.spawn()
    .add_component<Text>(utils::INTER_SEMI_BOLD, "Selected entity:", u32(16), 1.f)
    .add_component<Colour>(colour::white())
    .add_component<layout::Margin>(10.f, 0.f, 0.f, 10.f)
    .entity();
}

inline entt::entity spawn_entity_transform_information(AppCommands& app_commands)
{
  using utils::u32;
  auto const title = app_commands.spawn()
                       .add_component<Text>(utils::INTER_SEMI_BOLD, "Transform: ", u32(16), 1.f)
                       .add_component<Colour>(colour::white())
                       .entity();
  auto const value =
    app_commands.spawn()
      .add_component<Text>(utils::INTER_SEMI_BOLD, vector_utils::stringify(sf::Vector2f{ 10.f, 20.f }), u32(16), 1.f)
      .add_component<Colour>(colour::white())
      .entity();

  return app_commands.spawn()
    .add_component<layout::Flex>(layout::Flex::Direction::Horizontal, layout::Flex::Alignment::Center)
    .add_component<Children>(std::vector{ title, value })
    .add_component<layout::Height>(10.f)
    .add_component<layout::Margin>(10.f, 0.f, 0.f, 10.f)
    .entity();
}

inline entt::entity spawn_entity_size_information(AppCommands& app_commands)
{
  using utils::u32;
  auto const title = app_commands.spawn()
                       .add_component<Text>(utils::INTER_SEMI_BOLD, "Size: ", u32(16), 1.f)
                       .add_component<Colour>(colour::white())
                       .entity();
  auto const value =
    app_commands.spawn()
      .add_component<Text>(utils::INTER_SEMI_BOLD, vector_utils::stringify(sf::Vector2f{ 10.f, 20.f }), u32(16), 1.f)
      .add_component<Colour>(colour::white())
      .entity();

  return app_commands.spawn()
    .add_component<layout::Flex>(layout::Flex::Direction::Horizontal, layout::Flex::Alignment::Center)
    .add_component<Children>(std::vector{ title, value })
    .add_component<layout::Height>(10.f)
    .add_component<layout::Margin>(12.f, 0.f, 0.f, 10.f)
    .entity();
}


}// namespace debug::ui
