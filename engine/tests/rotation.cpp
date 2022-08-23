#pragma once

#include <SFML/Graphics.hpp>
#include <gtest/gtest.h>

#include "../rotation.h"

TEST(Rotation, RotationsAboutPoint)
{
  auto const error_threshold = 0.05f;

  {
    auto const result = rotation::rotate_about_point(sf::Vector2f{ 2.f, 2.f }, sf::Vector2f{ 0.f, 0.f }, 90);

    EXPECT_NEAR(result.x, -2.f, error_threshold);
    EXPECT_NEAR(result.y, 2.f, error_threshold);
  }

  {
    auto const result = rotation::rotate_about_point(sf::Vector2f{ 0.f, 1.f }, sf::Vector2f{ 0.f, 0.f }, 10);

    EXPECT_NEAR(result.x, -0.17365f, error_threshold);
    EXPECT_NEAR(result.y, 0.98481f, error_threshold);
  }
}
