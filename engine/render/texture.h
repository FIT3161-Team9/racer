#pragma once

#include <SFML/Graphics.hpp>

#include "../colour.h"
#include "../render_context.h"
#include "../texture.h"
#include "../transform.h"

namespace render
{

inline void
  texture(RenderContext& render_context, sf::RenderWindow& window, Transform const& transform, Texture const& texture)
{
  sf::Sprite sprite{};
  sprite.setTexture(render_context.get_or_load_texture(texture));
  sprite.setPosition(window::to_screen_space(transform.value));

  // TODO : add support for sprite sheets
  // sprite.setTextureRect(sf::IntRect(0, 8, 16, 16));
  // TODO : handle rendering a texture according to a given size
  // sprite.setScale( scale_that_will_set_it_to_given_size );

  window.draw(sprite);
}

}// namespace render