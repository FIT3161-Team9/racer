#pragma once

#include <cstring>

struct Arguments
{
  bool debug_mode;
};

namespace arguments
{

inline Arguments parse(int argc, char** argv)
{
  auto arguments = Arguments{ .debug_mode = false };

  if (argc != 2) { return arguments; }

  arguments.debug_mode = strcmp(argv[1], "-d") || strcmp(argv[1], "--debug");

  return arguments;
}

}// namespace arguments
