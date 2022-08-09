#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>

#include "../engine/app_commands.h"
#include "../engine/query.h"

namespace timing
{

struct TimingInternal
{
  sf::Clock clock{};
};

struct ElapsedTime
{
  std::uint64_t elapsed_milliseconds{};
};

inline void plugin(AppCommands& app_commands)
{
  app_commands.add_resource<TimingInternal>();
  app_commands.add_resource<ElapsedTime>();

  app_commands.add_system(ResourceQuery<TimingInternal>{}, [&](auto& resource) {
    auto&& [_, timing] = resource;
    auto const elapsed = timing.clock.restart();
    std::uint64_t const elapsed_milliseconds = elapsed.asMilliseconds();
    auto& elapsed_time = *app_commands.get_resource<ElapsedTime>();
    elapsed_time.elapsed_milliseconds = elapsed_milliseconds;
  });
}

};// namespace timing
