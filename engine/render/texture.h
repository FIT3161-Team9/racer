#pragma once

#include <SFML/Graphics.hpp>

#include "../colour.h"
#include "../render_context.h"
#include "../scale.h"
#include "../texture.h"
#include "../transform.h"
#include "../vector_utils.h"

namespace render
{

inline void texture(RenderContext& render_context,
                    sf::RenderWindow& window,
                    Transform const& transform,
                    Texture const& texture,
                    Scale const& scale,
                    Rotation const& rotation)
{
  sf::Sprite sprite{};
  sprite.setTexture(render_context.get_or_load_texture(texture));
  sprite.setScale(scale.scale);

  auto const bounding_box = sprite.getLocalBounds();
  sprite.setOrigin(sf::Vector2f{ bounding_box.width * 0.5f, bounding_box.height * 0.5f });

  sprite.setPosition(window::to_screen_space(transform.value));

  sprite.rotate(rotation.degrees);

  window.draw(sprite);
}

}// namespace render
