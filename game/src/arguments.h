#pragma once

#include <cstring>
#include <iostream>

struct Arguments
{
  bool debug_mode;
  char* level;
};

namespace arguments
{

inline Arguments parse(int argc, char** argv)
{
  auto arguments = Arguments{ .debug_mode = false };

  if (argc < 2) { return arguments; }

  int i = 1;
  while (i < argc) {
    if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) { arguments.debug_mode = true; }

    if (strcmp(argv[i], "--level") == 0 && i < argc - 1) {
      arguments.level = argv[i + 1];
      i += 2;
      continue;
    }

    i++;
  }


  return arguments;
}

}// namespace arguments
