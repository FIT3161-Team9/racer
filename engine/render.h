#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "circle.h"
#include "colour.h"
#include "render_context.h"
#include "texture.h"
#include "transform.h"
#include "window.h"

namespace render
{
void circle(sf::RenderWindow&, Transform const&, Circle const&, Colour const&);
void texture(RenderContext&, sf::RenderWindow&, Transform const&, Texture const&);

void all(RenderContext& render_context, sf::RenderWindow& window, entt::registry& registry)
{
  // TODO : Render according to a Z Order?
  auto circle_view = registry.view<Transform const, Circle const, Colour const>();
  circle_view.each([&](auto const& transform, auto const& circle, auto const& colour) {
    render::circle(window, transform, circle, colour);
  });
  auto texture_view = registry.view<Transform const, Texture const>();
  texture_view.each(
    [&](auto const& transform, auto const& texture) { render::texture(render_context, window, transform, texture); });
}

void circle(sf::RenderWindow& window, Transform const& transform, Circle const& circle, Colour const& colour)
{
  sf::CircleShape circle_shape(circle.radius);
  // We subtract the radius of the circle because SFML will render the circle relative to the top left,
  // but we want to render it relative to it's center
  circle_shape.setPosition(window::to_screen_space(transform.value) - sf::Vector2f{ circle.radius, circle.radius });
  circle_shape.setFillColor(sf::Color(colour.r, colour.g, colour.b, colour.a));
  window.draw(circle_shape);
}

void texture(RenderContext& render_context,
             sf::RenderWindow& window,
             Transform const& transform,
             Texture const& texture)
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

};// namespace render