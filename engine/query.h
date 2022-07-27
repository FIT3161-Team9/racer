#pragma once

#include <tuple>

template<typename... Components>
struct Query
{
  using types = ::std::tuple<Components...>;
};

template<typename... Resources>
struct ResourceQuery
{
  using types = ::std::tuple<Resources...>;
};
