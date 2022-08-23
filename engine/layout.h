#pragma once

namespace layout
{

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


}// namespace layout
