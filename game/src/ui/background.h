#pragma once

#include <SFML/Graphics.hpp>

#include "../utils.h"
#include "engine/circle.h"
#include <engine/app_commands.h>
#include <engine/colour.h>
#include <engine/entity.h>
#include <engine/rectangle.h>
#include <engine/transform.h>
#include <engine/window.h>

namespace background
{

inline Entity spawn(AppCommands& app_commands)
{
  using utils::u8;
  return app_commands.spawn()
    .add_component<Rectangle>(sf::Vector2f{ window::COORDINATE_SPACE_WIDTH, window::COORDINATE_SPACE_HEIGHT })
    .add_component<Colour>(u8(255), u8(237), u8(237))
    .add_component<Transform>(sf::Vector2f{ 0.f, 0.f })
    .add_component<ZIndex>(0);
};

inline void destroy(AppCommands& app_commands, entt::entity background) { app_commands.destroy(background); };

}// namespace background
