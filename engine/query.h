#pragma once

#include <tuple>

/// Query to use as a parameter of a System in the ECS. An iterator
/// over all entities with the given components will be passed into
/// the system
template<typename... Components>
struct Query
{
  using types = ::std::tuple<Components...>;
};

/// Query to use as a parameter of a System in the ECS. A tuple with
/// all of the given resources will be passed into the system
template<typename... Resources>
struct ResourceQuery
{
  using types = ::std::tuple<Resources...>;
};
