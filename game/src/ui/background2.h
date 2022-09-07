#pragma once

#include <SFML/Graphics.hpp>

#include <engine/app_commands.h>
#include <engine/colour.h>
#include <engine/entity.h>
#include <engine/rectangle.h>
#include <engine/transform.h>
#include <engine/window.h>
#include "engine/circle.h"
#include "../utils.h"

namespace background2
{

inline Entity spawn(AppCommands& app_commands)
{
  using utils::u8;
  return app_commands.spawn()
    .add_component<Rectangle>(sf::Vector2f{ 200,200})
    .add_component<Colour>(colour::orange())
    .add_component<Transform>(sf::Vector2f{ 0.f, 0.f })
    .add_component<Outline>(colour::blue(),300.f);
};

inline void destroy(AppCommands& app_commands, entt::entity background2) { app_commands.destroy(background2); };

}// namespace background