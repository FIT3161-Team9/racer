#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <entt/entt.hpp>

#include "SFML/System/Vector2.hpp"
#include "children.h"
#include "circle.h"
#include "colour.h"
#include "layout.h"
#include "outline.h"
#include "rectangle.h"
#include "render_context.h"
#include "render_utils.h"
#include "rotation.h"
#include "text.h"
#include "texture.h"
#include "transform.h"
#include "triangle.h"
#include "vector_utils.h"
#include "window.h"

namespace render
{

void circle(sf::RenderWindow&, Transform const&, Circle const&, Colour const&);
void rectangle(RenderContext&, sf::RenderWindow&, entt::entity, Transform const&, Rectangle const&, Colour const&);
void texture(RenderContext&, sf::RenderWindow&, Transform const&, Texture const&);
void text(RenderContext&, sf::RenderWindow&, Transform const&, Text const&, Colour const&);
void vector(sf::RenderWindow&, sf::Vector2f const&, Transform const&, Colour const&);
void triangle(RenderContext&, sf::RenderWindow&, entt::entity, Transform const&, Triangle const&, Colour const&);
void flex_box(RenderContext&, sf::RenderWindow&, entt::entity, layout::Flex const&, Children const&);
void flex_box_vertical(RenderContext&, sf::RenderWindow&, entt::entity, layout::Flex const&, Children const&);
void flex_box_horizontal(RenderContext&, sf::RenderWindow&, entt::entity, layout::Flex const&, Children const&);


/// Render all entities in the ECS that meet the criteria to be rendered. See the parameters of each
/// of the individual render methods to see what the criteria is to be rendered
inline void all(RenderContext& render_context, sf::RenderWindow& window, entt::registry& registry)
{
  // TODO : Render according to a Z Order?
  auto texture_view = registry.view<Transform const, Texture const>();
  texture_view.each(
    [&](auto const& transform, auto const& texture) { render::texture(render_context, window, transform, texture); });
  auto rectangle_view = registry.view<Transform const, Rectangle const, Colour const>();
  for (auto [entity, transform, rectangle, colour] : rectangle_view.each()) {
    render::rectangle(render_context, window, entity, transform, rectangle, colour);
  }
  auto triangle_view = registry.view<Transform const, Colour const, Triangle const>();
  for (auto [entity, transform, colour, triangle] : triangle_view.each()) {
    render::triangle(render_context, window, entity, transform, triangle, colour);
  }
  auto circle_view = registry.view<Transform const, Circle const, Colour const>();
  circle_view.each([&](auto const& transform, auto const& circle, auto const& colour) {
    render::circle(window, transform, circle, colour);
  });
  auto text_view = registry.view<Transform const, Text const, Colour const>();
  for (auto [_entity, transform, text, colour] : text_view.each()) {
    render::text(render_context, window, transform, text, colour);
  }
  auto flex_boxes = registry.view<layout::Flex, Children>();
  for (auto [flex_parent, flex, children] : flex_boxes.each()) {
    render::flex_box(render_context, window, flex_parent, flex, children);
  }
}

inline void flex_box(RenderContext& render_context,
                     sf::RenderWindow& window,
                     entt::entity flex_parent,
                     layout::Flex const& layout,
                     Children const& children)
{
  if (layout.direction == layout::Flex::Direction::Vertical) {
    render::flex_box_vertical(render_context, window, flex_parent, layout, children);
  }
  if (layout.direction == layout::Flex::Direction::Horizontal) {
    render::flex_box_horizontal(render_context, window, flex_parent, layout, children);
  }
}

inline void rectangle(RenderContext& render_context,
                      sf::RenderWindow& window,
                      entt::entity entity,
                      Transform const& transform,
                      Rectangle const& rectangle,
                      Colour const& colour)
{
  sf::RectangleShape rectangle_shape{};
  rectangle_shape.setOrigin(sf::Vector2f{ 0.5f * rectangle.width_height.x, 0.5f * rectangle.width_height.y });
  rectangle_shape.setPosition(window::to_screen_space(transform.value));
  rectangle_shape.setSize(rectangle.width_height);
  rectangle_shape.setFillColor(sf::Color(colour.r, colour.g, colour.b, colour.a));
  auto* outline = render_context.get_component<Outline>(entity);
  if (outline) {
    auto outline_colour = outline->colour;
    rectangle_shape.setOutlineColor(sf::Color(outline_colour.r, outline_colour.g, outline_colour.b, outline_colour.a));
    rectangle_shape.setOutlineThickness(outline->thickness);
  }
  auto* rotation_component = render_context.get_component<Rotation>(entity);
  auto rotation = rotation_component ? rotation_component->degrees : 0.f;
  if (rotation != 0.f) { rectangle_shape.rotate(rotation); }
  window.draw(rectangle_shape);
};

inline void triangle(RenderContext& render_context,
                     sf::RenderWindow& window,
                     entt::entity entity,
                     Transform const& transform,
                     Triangle const& triangle,
                     Colour const& colour)
{
  (void)render_context;
  (void)entity;
  sf::ConvexShape sf_triangle{};
  sf_triangle.setPointCount(3);
  sf_triangle.setPoint(0, window::to_screen_space(transform.value) - triangle.point_one);
  sf_triangle.setPoint(1, window::to_screen_space(transform.value) - triangle.point_two);
  sf_triangle.setPoint(2, window::to_screen_space(transform.value) - triangle.point_three);
  sf_triangle.setFillColor(render_utils::convert_colour(colour));
  window.draw(sf_triangle);
}

inline void circle(sf::RenderWindow& window, Transform const& transform, Circle const& circle, Colour const& colour)
{
  sf::CircleShape circle_shape(circle.radius);
  // We subtract the radius of the circle because SFML will render the circle relative to the top left,
  // but we want to render it relative to it's center
  circle_shape.setPosition(window::to_screen_space(transform.value) - sf::Vector2f{ circle.radius, circle.radius });
  circle_shape.setFillColor(render_utils::convert_colour(colour));
  window.draw(circle_shape);
}

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

inline void vector(sf::RenderWindow& window, sf::Vector2f const& vec, Transform const& transform, Colour const& colour)
{
  sf::RectangleShape rectangle_shape{};
  rectangle_shape.setSize({ vector_utils::magnitude(vec), 200.f });
  rectangle_shape.setFillColor(render_utils::convert_colour(colour));
  rectangle_shape.setPosition(transform.value);
  rectangle_shape.setRotation(
    rotation::to_radians(std::acos(vector_utils::dot_product(vec, { 0.f, 1.f }) / vector_utils::magnitude(vec))));
  window.draw(rectangle_shape);
}

inline void flex_box_vertical(RenderContext& render_context,
                              sf::RenderWindow& window,
                              entt::entity flex_parent,
                              layout::Flex const& layout,
                              Children const& children)
{
  auto vertical_space_used_so_far = 0.f;
  auto edge_bound_x = 0.f;

  for (auto child : children.children) {
    auto* text = render_context.get_component<Text>(child);
    auto* colour = render_context.get_component<Colour>(child);
    auto* margin = render_context.get_component<layout::Margin>(child);
    sf::Text sf_text{};
    sf_text.setFont(render_context.get_or_load_font(*text));
    sf_text.setCharacterSize(text->character_size);
    sf_text.setString(text->content.data());
    sf_text.setFillColor(render_utils::convert_colour(*colour));
    sf_text.setLetterSpacing(text->letter_spacing);

    if (margin && margin->top) { vertical_space_used_so_far += margin->top; }

    // First layout - this doesn't work because positioning text considers the tallest
    // letter in the font (we only want to consider the tallest letter in this string)
    auto const bounds = sf_text.getGlobalBounds();

    if (layout.alignment == layout::Flex::Alignment::Center) {
      edge_bound_x = window::COORDINATE_SPACE_WIDTH * 0.5 - 0.5 * bounds.width;
    }
    if (layout.alignment == layout::Flex::Alignment::Start) { edge_bound_x = 0.f; }
    if (margin && margin->left && layout.alignment == layout::Flex::Alignment::Start) { edge_bound_x += margin->left; }

    sf_text.setPosition(edge_bound_x, vertical_space_used_so_far);

    // Re-layout - after the first layout we now know the difference between the tallest letter in the font
    // and the tallest letter we're using. We need to offset by this distance
    auto const bounds_after_setting_position = sf_text.getGlobalBounds();
    auto const difference_between_target_and_actual = bounds_after_setting_position.top - vertical_space_used_so_far;
    sf_text.setPosition(edge_bound_x, vertical_space_used_so_far - difference_between_target_and_actual);

    window.draw(sf_text);

    if (margin && margin->bottom) { vertical_space_used_so_far += margin->bottom; }

    vertical_space_used_so_far += bounds.height;
  }
  (void)flex_parent;
}

inline void flex_box_horizontal(RenderContext& render_context,
                                sf::RenderWindow& window,
                                entt::entity flex_parent,
                                layout::Flex const& layout,
                                Children const& children)
{
  auto horizontal_space_used_so_far = 50.f;
  auto edge_bound_y = 0.f;

  for (auto child : children.children) {
    auto* text = render_context.get_component<Text>(child);
    auto* colour = render_context.get_component<Colour>(child);
    auto* margin = render_context.get_component<layout::Margin>(child);
    sf::Text sf_text{};
    sf_text.setFont(render_context.get_or_load_font(*text));
    sf_text.setCharacterSize(text->character_size);
    sf_text.setString(text->content.data());
    sf_text.setFillColor(render_utils::convert_colour(*colour));
    sf_text.setLetterSpacing(text->letter_spacing);

    if (margin && margin->left) { horizontal_space_used_so_far += margin->left; }

    // First layout - this doesn't work because positioning text considers the tallest
    // letter in the font (we only want to consider the tallest letter in this string)
    auto const bounds = sf_text.getGlobalBounds();

    if (layout.alignment == layout::Flex::Alignment::Center) {
      edge_bound_y = window::COORDINATE_SPACE_HEIGHT * 0.5 - 0.5 * bounds.height;
    }

    if (layout.alignment == layout::Flex::Alignment::Start) { edge_bound_y = 50.f - 0.5 * bounds.height; }

    if (layout.alignment == layout::Flex::Alignment::End) {
      edge_bound_y = window::COORDINATE_SPACE_HEIGHT * 0.95 - 0.5 * bounds.height;
    }

    sf_text.setPosition(horizontal_space_used_so_far, edge_bound_y);

    // Re-layout - after the first layout we now know the difference between the tallest letter in the font
    // and the tallest letter we're using. We need to offset by this distance
    auto const bounds_after_setting_position = sf_text.getGlobalBounds();
    auto const difference_between_target_and_actual = bounds_after_setting_position.left - horizontal_space_used_so_far;
    edge_bound_y -= 0.5 * bounds_after_setting_position.height;
    sf_text.setPosition(horizontal_space_used_so_far - difference_between_target_and_actual, edge_bound_y);

    window.draw(sf_text);

    if (margin && margin->right) { horizontal_space_used_so_far += margin->right; }

    horizontal_space_used_so_far += bounds.width;
  }
  (void)flex_parent;
}

};// namespace render
