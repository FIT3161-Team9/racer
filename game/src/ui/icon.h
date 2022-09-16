#pragma once

#include <SFML/Graphics.hpp>

#include "../utils.h"
#include <engine/app_commands.h>
#include <engine/colour.h>
#include <engine/entity.h>
#include <engine/triangle.h>
#include <engine/transform.h>
#include <engine/window.h>
#include <engine/zindex.h>

namespace icon
{

inline Entity spawn(AppCommands& app_commands, sf::Vector2f transform)
{
  using utils::u8;
  return app_commands.spawn()
    .add_component<Triangle>(sf::Vector2f{ 50.f, 0.f }, sf::Vector2f{ 50.f, 60.f }, sf::Vector2f{ 90.f, 30.f })
    .add_component<Colour>(colour::black())
    .add_component<Transform>(transform);
    // .add_component<Outline>(colour::blue(), 300.f)
    // .add_component<ZIndex>(1);
};

inline void destroy(AppCommands& app_commands, entt::entity icon) { app_commands.destroy(icon); };

}// namespace background
