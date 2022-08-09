#pragma once

#include <entt/entt.hpp>
#include <vector>

/// A basic component for associating a parent-child relationship
struct Children
{
  std::vector<entt::entity> children;
};
