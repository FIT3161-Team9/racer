#pragma once

#include <SFML/Graphics.hpp>
#include <gtest/gtest.h>

#include "../rotation.h"

TEST(Rotation, RotationsAboutPoint)
{
  auto const result = rotation::rotate_about_point(sf::Vector2f{ 2.f, 2.f }, sf::Vector2f{ 0.f, 0.f }, 90);

  EXPECT_FLOAT_EQ(result.x, 2.f);
  EXPECT_FLOAT_EQ(result.y, -2.f);
}
