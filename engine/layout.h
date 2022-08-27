#pragma once

#include <SFML/Graphics.hpp>

#include "render_context.h"
#include "render_utils.h"
#include "text.h"

namespace layout
{

/// Limit the height of a component to a specific value when laying it out
struct Height
{
  float value;
};

/// Limit the width of a component to a specific value when laying it out
struct Width
{
  float value;
};

/// The root of the UI. Rendering will start here, and traverse the root's children
enum class FlexRoot {};

/// Component for flex box layout parent - render lays out the children of the entity with this
/// component in a CSS Flex-box like fashion
struct Flex
{
  /// The direction to layout children
  enum class Direction { Vertical, Horizontal };

  /// Alignment of children
  enum class Alignment { Start, Center, End };

  Direction direction;
  Alignment alignment;
};

/// Margin to be used when laying out an item
struct Margin
{
  float top{};
  float right{};
  float bottom{};
  float left{};
};

inline Margin margin_top(float value) { return Margin{ .top = value }; }
inline Margin margin_left(float value) { return Margin{ .left = value }; }

struct FlexContext
{
  float current_x{};
  float current_y{};
  float width{};
  float height{};
  layout::Flex layout;
};

struct Result
{
  float horizontal_space_used{};
  float vertical_space_used{};
};

inline Result layout_text(RenderContext& render_context,
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

  auto const margin_top = margin ? margin->top : 0.f;
  auto const margin_left = margin ? margin->left : 0.f;
  auto const margin_bottom = margin ? margin->bottom : 0.f;
  auto const margin_right = margin ? margin->right : 0.f;

  auto const bounds = sf_text.getGlobalBounds();
  auto const alignment_padding_top = [&]() {
    if (flex_context.layout.alignment != layout::Flex::Alignment::Center) { return 0.f; }
    if (flex_context.layout.direction != layout::Flex::Direction::Horizontal) { return 0.f; }
    return 0.5f * flex_context.height - 0.5f * bounds.height;
  }();
  auto const alignment_padding_left = [&]() {
    if (flex_context.layout.alignment != layout::Flex::Alignment::Center) { return 0.f; }
    if (flex_context.layout.direction != layout::Flex::Direction::Vertical) { return 0.f; }
    return 0.5f * flex_context.width - 0.5f * bounds.width;
  }();
  auto const x_position = [&]() { return flex_context.current_x + margin_left + alignment_padding_left; }();
  auto const y_position = [&]() { return flex_context.current_y + margin_top + alignment_padding_top; }();

  // First layout - this doesn't work because positioning text considers the tallest
  // letter in the font (we only want to consider the tallest letter in this string)
  sf_text.setPosition(x_position, y_position);

  // Re-layout - after the first layout we now know the difference between the tallest letter in the font
  // and the tallest letter we're using. We need to offset by this distance
  auto const bounds_after_setting_position = sf_text.getGlobalBounds();

  auto const difference_between_target_and_actual_x = bounds_after_setting_position.left - x_position;
  auto const difference_between_target_and_actual_y = bounds_after_setting_position.top - y_position;

  sf_text.setPosition(x_position - difference_between_target_and_actual_x,
                      y_position - difference_between_target_and_actual_y);

  window.draw(sf_text);

  return Result{ .horizontal_space_used = margin_left + 2.f * alignment_padding_left + bounds.width + margin_right,
                 .vertical_space_used = margin_top + 2.f * alignment_padding_top + bounds.height + margin_bottom };
}

inline Result layout_flex(RenderContext& render_context,
                          sf::RenderWindow& window,
                          FlexContext const& parent_flex_context,
                          layout::Flex const& flex_box,
                          entt::entity entity);

/// Given an entity that is the child of a flex box, layout the child
/// within the flex box based on the component on the child
inline Result layout_child(RenderContext& render_context,
                           sf::RenderWindow& window,
                           FlexContext const& flex_context,
                           entt::entity child)
{
  auto* text = render_context.get_component<Text>(child);
  if (text) { return layout_text(render_context, window, flex_context, *text, child); }

  auto* flex = render_context.get_component<layout::Flex>(child);
  if (flex) { return layout_flex(render_context, window, flex_context, *flex, child); }

  assert(!"Unsupported component in flex box");
}

inline Result layout_flex(RenderContext& render_context,
                          sf::RenderWindow& window,
                          FlexContext const& parent_flex_context,
                          layout::Flex const& flex_box,
                          entt::entity entity)
{
  auto* children_ptr = render_context.get_component<Children>(entity);
  assert(children_ptr && "Flex box has no children");
  auto const& children = *children_ptr;

  auto flex_context = FlexContext{ .current_y = parent_flex_context.current_y,
                                   .current_x = parent_flex_context.current_x,
                                   .layout = flex_box };

  auto* width = render_context.get_component<layout::Width>(entity);
  auto* height = render_context.get_component<layout::Height>(entity);
  flex_context.height = height ? height->value : parent_flex_context.height;
  flex_context.width = width ? width->value : parent_flex_context.width;

  auto* margin = render_context.get_component<layout::Margin>(entity);
  auto const margin_left = margin ? margin->left : 0.f;
  auto const margin_right = margin ? margin->right : 0.f;
  auto const margin_top = margin ? margin->top : 0.f;
  auto const margin_bottom = margin ? margin->bottom : 0.f;

  flex_context.current_y += margin_top;
  flex_context.current_x += margin_left;

  // The width of this flex boxes widest child
  float widest_child_width = 0.f;
  float tallest_child_height = 0.f;

  for (auto child : children.children) {
    auto const layout_result = layout_child(render_context, window, flex_context, child);

    tallest_child_height = std::max(tallest_child_height, layout_result.vertical_space_used);
    widest_child_width = std::max(widest_child_width, layout_result.horizontal_space_used);
    if (flex_box.direction == layout::Flex::Direction::Vertical) {
      flex_context.current_y += layout_result.vertical_space_used;
    } else {
      flex_context.current_x += layout_result.horizontal_space_used;
    }
  }

  flex_context.current_y += margin_bottom;
  flex_context.current_x += margin_right;

  // The amount of space used by this flex box is calculated
  // differently depending on the direction
  auto const horizontal_space_used = [&]() {
    if (flex_box.direction == layout::Flex::Direction::Horizontal) {
      return flex_context.current_x - parent_flex_context.current_x;
    }
    return margin_left + widest_child_width + margin_right;
  }();

  auto const vertical_space_used = [&]() {
    if (flex_box.direction == layout::Flex::Direction::Vertical) {
      return flex_context.current_y - parent_flex_context.current_y;
    }
    return margin_top + tallest_child_height + margin_bottom;
  }();

  return Result{ .horizontal_space_used = horizontal_space_used, .vertical_space_used = vertical_space_used };
}

}// namespace layout
