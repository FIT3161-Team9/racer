#pragma once

#include <SFML/Graphics.hpp>

#include "../colour.h"
#include "../render_context.h"
#include "../text.h"
#include "../transform.h"

namespace render
{

inline void text(RenderContext& render_context,
                 sf::RenderWindow& window,
                 Transform const& transform,
                 Text const& text,
                 Colour const& colour)
{
  sf::Text sf_text{};
  sf_text.setFont(render_context.get_or_load_font(text));
  sf_text.setPosition(window::to_screen_space(transform.value));
  sf_text.setCharacterSize(text.character_size);
  sf_text.setLetterSpacing(text.letter_spacing);
  sf_text.setString(text.content.data());
  sf_text.setFillColor(render_utils::convert_colour(colour));

  window.draw(sf_text);
}

}// namespace render