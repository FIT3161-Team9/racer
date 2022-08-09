#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>

#include "../engine/app_commands.h"
#include "../engine/query.h"

namespace timing
{

/// Component stored as a resource to keep track of time
struct TimingInternal
{
  sf::Clock clock{};
};

/// Resource that represents the elapsed milliseconds since the last
/// iteration of the main app loop. It is stored as a resource and kept
/// up to date when the timing plugin is used
struct ElapsedTime
{
  std::uint64_t elapsed_milliseconds{};
};

/// Plugin for keeping track of the amount of time elapsed between
/// iterations of the main app loop. Provides the `ElapsedTime` resource
inline void plugin(AppCommands& app_commands)
{
  app_commands.add_resource<TimingInternal>();
  app_commands.add_resource<ElapsedTime>();

  // This system keeps the ElapsedTime resource up to date with the
  // amount of time passed between main loop iterations
  app_commands.add_system(ResourceQuery<TimingInternal>{}, [&](auto& resource) {
    auto&& [_, timing] = resource;
    auto const elapsed = timing.clock.restart();
    std::uint64_t const elapsed_milliseconds = elapsed.asMilliseconds();
    auto& elapsed_time = *app_commands.get_resource<ElapsedTime>();
    elapsed_time.elapsed_milliseconds = elapsed_milliseconds;
  });
}

};// namespace timing
