#pragma once

#include "../vector_utils.h"
#include <SFML/Graphics.hpp>
#include <gtest/gtest.h>

TEST(VectorUtils, Magnitude)
{
  EXPECT_FLOAT_EQ(vector_utils::magnitude(sf::Vector2f{ 2.f, 7.f }), 7.28011f);
  EXPECT_FLOAT_EQ(vector_utils::magnitude(sf::Vector2f{ 232.f, 60.f }), 239.63305f);
  EXPECT_FLOAT_EQ(vector_utils::magnitude(sf::Vector2f{ 0.f, 0.f }), 0.f);
  EXPECT_FLOAT_EQ(vector_utils::magnitude(sf::Vector2f{ -10.f, -10.f }), 14.1421356237f);
  EXPECT_FLOAT_EQ(vector_utils::magnitude(sf::Vector2f{ 10.f, -10.f }), 14.1421356237f);
}

TEST(VectorUtils, DotProduct)
{
  EXPECT_FLOAT_EQ(vector_utils::dot_product(sf::Vector2f{ 0.f, 0.f }, sf::Vector2f{ 0.f, 0.f }), 0.f);
  EXPECT_FLOAT_EQ(vector_utils::dot_product(sf::Vector2f{ 1.f, 2.f }, sf::Vector2f{ 2.f, 1.f }), 4.f);
  EXPECT_FLOAT_EQ(vector_utils::dot_product(sf::Vector2f{ 3.f, 5.f }, sf::Vector2f{ -2.f, 10.f }), 44.f);
  EXPECT_FLOAT_EQ(vector_utils::dot_product(sf::Vector2f{ 3.f, 5.f }, sf::Vector2f{ -2.f, 10.23f }), 45.15f);
}

TEST(VectorUtils, Scale)
{
  {
    auto const result = vector_utils::scale_vector(sf::Vector2f{ 1.f, 2.f }, 8.f);
    EXPECT_FLOAT_EQ(result.x, 8.f);
    EXPECT_FLOAT_EQ(result.y, 16.f);
  }

  {
    auto const result = vector_utils::scale_vector(sf::Vector2f{ 18.f, 7.f }, 0.f);
    EXPECT_FLOAT_EQ(result.x, 0.f);
    EXPECT_FLOAT_EQ(result.y, 0.f);
  }

  {
    auto const result = vector_utils::scale_vector(sf::Vector2f{ 768951.f, 0.f }, 1.897f);
    EXPECT_FLOAT_EQ(result.x, 1458700.047f);
    EXPECT_FLOAT_EQ(result.y, 0.f);
  }
}

TEST(VectorUtils, Distance)
{
  {
    auto const result = vector_utils::distance_between(sf::Vector2f{ -7.f, -4.f }, sf::Vector2f{ 17.f, 6.5f });
    EXPECT_FLOAT_EQ(result, 26.196374f);
  }

  {
    auto const result = vector_utils::distance_between(sf::Vector2f{ 7.f, 4.f }, sf::Vector2f{ 7.f, 4.f });
    EXPECT_FLOAT_EQ(result, 0.f);
  }
}
