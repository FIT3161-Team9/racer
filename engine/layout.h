#pragma once

namespace layout
{

struct Flex
{
  enum class Direction { Vertical, Horizontal };

  enum class Alignment { Start, Center, End };

  Direction direction;
  Alignment alignment;
};

struct Margin
{
  float top{};
  float right{};
  float bottom{};
  float left{};
};

inline Margin margin_top(float value) { return Margin{ value }; }

}// namespace layout
