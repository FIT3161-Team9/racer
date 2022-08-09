#pragma once

namespace layout
{

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

inline Margin margin_top(float value) { return Margin{ value }; }

}// namespace layout