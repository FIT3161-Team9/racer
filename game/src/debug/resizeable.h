#pragma once

#include <vector>

#include <entt/entt.hpp>

namespace debug
{

struct Resizeable
{
  bool is_resizing;
  // The circles that the user can grap to resize the element
  std::vector<entt::entity> resize_circles{};
};

}// namespace debug
