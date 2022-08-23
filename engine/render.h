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

// TODO: Give this thing a nicer home
struct LayoutResult
{
  float horizontal_space_used{};
  float vertical_space_used{};
};

// TODO: Give this thing a nicer home
struct FlexContext
{
  float current_x{};
  float current_y{};
  layout::Flex layout;
};


void circle(sf::RenderWindow&, Transform const&, Circle const&, Colour const&);
void rectangle(RenderContext&, sf::RenderWindow&, entt::entity, Transform const&, Rectangle const&, Colour const&);
void texture(RenderContext&, sf::RenderWindow&, Transform const&, Texture const&);
void text(RenderContext&, sf::RenderWindow&, Transform const&, Text const&, Colour const&);
void vector(sf::RenderWindow&, sf::Vector2f const&, Transform const&, Colour const&);
void triangle(RenderContext&, sf::RenderWindow&, entt::entity, Transform const&, Triangle const&, Colour const&);
void root_flex_box(RenderContext&, sf::RenderWindow&, entt::entity, layout::Flex const&, Children const&);
LayoutResult flex_box_vertical(RenderContext&,
                               sf::RenderWindow&,
                               entt::entity,
                               layout::Flex const&,
                               Children const&,
                               FlexContext const* parent_flex_context);
LayoutResult flex_box_horizontal(RenderContext&,
                                 sf::RenderWindow&,
                                 entt::entity,
                                 layout::Flex const&,
                                 Children const&,
                                 FlexContext const* parent_flex_context);


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
  auto flex_boxes = registry.view<layout::FlexRoot, layout::Flex, Children>();
  for (auto [flex_parent, _flex_root, flex, children] : flex_boxes.each()) {
    render::root_flex_box(render_context, window, flex_parent, flex, children);
  }
}

inline void root_flex_box(RenderContext& render_context,
                          sf::RenderWindow& window,
                          entt::entity flex_parent,
                          layout::Flex const& layout,
                          Children const& children)
{
  if (layout.direction == layout::Flex::Direction::Vertical) {
    render::flex_box_vertical(render_context, window, flex_parent, layout, children, nullptr);
  }
  if (layout.direction == layout::Flex::Direction::Horizontal) {
    render::flex_box_horizontal(render_context, window, flex_parent, layout, children, nullptr);
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
inline LayoutResult layout_text(RenderContext& render_context,
                                sf::RenderWindow& window,
                                FlexContext const& flex_context,
                                Text const& text,
                                entt::entity entity)
{
  auto* colour = render_context.get_component<Colour>(entity);
  auto* margin = render_context.get_component<layout::Margin>(entity);
  sf::Text sf_text{};
  sf_text.setFont(render_context.get_or_load_font(text));
  sf_text.setCharacterSize(text.character_size);
  sf_text.setString(text.content.data());
  sf_text.setFillColor(render_utils::convert_colour(*colour));
  sf_text.setLetterSpacing(text.letter_spacing);

  auto const margin_top = [margin]() { return margin ? margin->top : 0.f; };
  auto const margin_left = [margin]() { return margin ? margin->left : 0.f; };
  auto const margin_bottom = [margin]() { return margin ? margin->bottom : 0.f; };
  auto const margin_right = [margin]() { return margin ? margin->right : 0.f; };

  auto const alignment_padding_top = [&](auto const& bounds) {
    if (flex_context.layout.alignment != layout::Flex::Alignment::Center) { return 0.f; }
    if (flex_context.layout.direction != layout::Flex::Direction::Horizontal) { return 0.f; }
    return -0.5f * bounds.height;
  };
  auto const alignment_padding_left = [&](auto const& bounds) {
    if (flex_context.layout.alignment != layout::Flex::Alignment::Center) { return 0.f; }
    if (flex_context.layout.direction != layout::Flex::Direction::Vertical) { return 0.f; }
    return 0.5f * window::COORDINATE_SPACE_WIDTH - 0.5f * bounds.width;
  };

  auto const x_position = [&](auto const& bounds) {
    return flex_context.current_x + margin_left() + alignment_padding_left(bounds);
  };
  auto const y_position = [&](auto const& bounds) {
    return flex_context.current_y + margin_top() + alignment_padding_top(bounds);
  };

  // First layout - this doesn't work because positioning text considers the tallest
  // letter in the font (we only want to consider the tallest letter in this string)
  auto const bounds = sf_text.getGlobalBounds();
  sf_text.setPosition(x_position(bounds), y_position(bounds));

  // Re-layout - after the first layout we now know the difference between the tallest letter in the font
  // and the tallest letter we're using. We need to offset by this distance
  auto const bounds_after_setting_position = sf_text.getGlobalBounds();
  auto const difference_between_target_and_actual_x =
    bounds_after_setting_position.left - x_position(bounds_after_setting_position);
  auto const difference_between_target_and_actual_y =
    bounds_after_setting_position.top - y_position(bounds_after_setting_position);

  sf_text.setPosition(x_position(bounds) - difference_between_target_and_actual_x,
                      y_position(bounds) - difference_between_target_and_actual_y);

  window.draw(sf_text);

  return LayoutResult{ .horizontal_space_used =
                         margin_left() + alignment_padding_left(bounds) + bounds.width + margin_right(),
                       .vertical_space_used =
                         margin_top() + alignment_padding_top(bounds) + bounds.height + margin_bottom() };
}

inline LayoutResult layout_flex(RenderContext& render_context,
                                sf::RenderWindow& window,
                                FlexContext const& parent_flex_context,
                                layout::Flex const& flex,
                                entt::entity entity)
{
  auto* children = render_context.get_component<Children>(entity);

  if (flex.direction == layout::Flex::Direction::Vertical) {
    return render::flex_box_vertical(render_context, window, entity, flex, *children, &parent_flex_context);
  }
  if (flex.direction == layout::Flex::Direction::Horizontal) {
    return render::flex_box_horizontal(render_context, window, entity, flex, *children, &parent_flex_context);
  }
  // We didn't use any space...
  return LayoutResult{};
}

inline LayoutResult flex_box_vertical(RenderContext& render_context,
                                      sf::RenderWindow& window,
                                      entt::entity flex_parent,
                                      layout::Flex const& layout,
                                      Children const& children,
                                      FlexContext const* parent_flex_context)
{
  auto flex_context = FlexContext{ .layout = layout };
  if (parent_flex_context) {
    flex_context.current_y = parent_flex_context->current_y;
    flex_context.current_x = parent_flex_context->current_x;
  }
  auto* margin = render_context.get_component<layout::Margin>(flex_parent);
  if (margin) {
    flex_context.current_y += margin->top;
    flex_context.current_x += margin->left;
  }
  // The width of this flex boxes widest child
  float widest_child_width = 0.f;

  for (auto child : children.children) {
    auto* text = render_context.get_component<Text>(child);
    auto* flex = render_context.get_component<layout::Flex>(child);
    if (text) {
      auto text_layout_result = layout_text(render_context, window, flex_context, *text, child);
      flex_context.current_y += text_layout_result.vertical_space_used;
      widest_child_width = std::max(widest_child_width, text_layout_result.horizontal_space_used);
    } else if (flex) {
      auto flex_layout_result = layout_flex(render_context, window, flex_context, *flex, child);
      flex_context.current_y += flex_layout_result.vertical_space_used;
      widest_child_width = std::max(widest_child_width, flex_layout_result.horizontal_space_used);
    }
  }

  if (margin) {
    flex_context.current_y += margin->bottom;
    flex_context.current_x += margin->right;
  }
  return LayoutResult{ .horizontal_space_used = widest_child_width, .vertical_space_used = flex_context.current_y };
}

inline LayoutResult flex_box_horizontal(RenderContext& render_context,
                                        sf::RenderWindow& window,
                                        entt::entity flex_parent,
                                        layout::Flex const& layout,
                                        Children const& children,
                                        FlexContext const* parent_flex_context)
{
  (void)flex_parent;

  auto flex_context = FlexContext{ .layout = layout };
  if (parent_flex_context) {
    flex_context.current_y = parent_flex_context->current_y;
    flex_context.current_x = parent_flex_context->current_x;
  }
  auto* margin = render_context.get_component<layout::Margin>(flex_parent);
  if (margin) {
    flex_context.current_y += margin->top;
    flex_context.current_x += margin->left;
  }
  // The width of this flex boxes widest child
  float tallest_child_height = 0.f;

  for (auto child : children.children) {
    auto* text = render_context.get_component<Text>(child);
    auto* flex = render_context.get_component<layout::Flex>(child);
    if (text) {
      auto text_layout_result = layout_text(render_context, window, flex_context, *text, child);
      flex_context.current_x += text_layout_result.horizontal_space_used;
      tallest_child_height = std::max(tallest_child_height, text_layout_result.horizontal_space_used);
    } else if (flex) {
      auto flex_layout_result = layout_flex(render_context, window, flex_context, *flex, child);
      flex_context.current_x += flex_layout_result.horizontal_space_used;
      tallest_child_height = std::max(tallest_child_height, flex_layout_result.horizontal_space_used);
    }
  }

  if (margin) {
    flex_context.current_y += margin->bottom;
    flex_context.current_x += margin->right;
  }

  return LayoutResult{ .horizontal_space_used = flex_context.current_x, .vertical_space_used = tallest_child_height };
}


};// namespace render
