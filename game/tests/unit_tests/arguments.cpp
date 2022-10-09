#include <gtest/gtest.h>

#include "game/src/arguments.h"

TEST(Arguments, NoFlags)
{
  char* args_one[1] = { const_cast<char*>("/path/to/executable") };
  auto const arguments = arguments::parse(1, args_one);

  EXPECT_FALSE(arguments.debug_mode);
  EXPECT_TRUE(arguments.level == nullptr);
}

TEST(Arguments, DebugMode)
{
  char* args_one[2] = { const_cast<char*>("/path/to/executable"), const_cast<char*>("--debug") };
  auto const arguments = arguments::parse(2, args_one);

  EXPECT_TRUE(arguments.debug_mode);
  EXPECT_TRUE(arguments.level == nullptr);
}

TEST(Arguments, Level)
{
  char* args_one[3] = { const_cast<char*>("/path/to/executable"),
                        const_cast<char*>("--level"),
                        const_cast<char*>("/path/to/level") };
  auto const arguments = arguments::parse(3, args_one);

  EXPECT_FALSE(arguments.debug_mode);
  EXPECT_EQ(arguments.level, "/path/to/level");
}

TEST(Arguments, DebugAndLevel)
{
  char* args_one[4] = {
    const_cast<char*>("/path/to/executable"),
    const_cast<char*>("--level"),
    const_cast<char*>("/path/to/level"),
    const_cast<char*>("--debug"),
  };
  auto const arguments = arguments::parse(4, args_one);

  EXPECT_TRUE(arguments.debug_mode);
  EXPECT_EQ(arguments.level, "/path/to/level");
}
