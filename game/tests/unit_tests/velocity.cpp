#pragma once

#include <SFML/Graphics.hpp>
#include <gtest/gtest.h>


#include "game/src/velocity.h"


TEST(ApplyVelocity, ZeroVelocity)
{
  auto const result =
    velocity::apply_velocity(Velocity{ sf::Vector2f{ 0.f, 0.f } }, Transform{ sf::Vector2f{ 0.f, 0.f } }, 100);
  EXPECT_FLOAT_EQ(result.value.x, 0.f);
  EXPECT_FLOAT_EQ(result.value.y, 0.f);
}

TEST(ApplyVelocity, ZeroTimePassed)
{
  auto const result =
    velocity::apply_velocity(Velocity{ sf::Vector2f{ 10.f, 10.f } }, Transform{ sf::Vector2f{ 0.f, 0.f } }, 0);
  EXPECT_FLOAT_EQ(result.value.x, 0.f);
  EXPECT_FLOAT_EQ(result.value.y, 0.f);
}

TEST(ApplyVelocity, OneSecondRight)
{
  auto const result =
    velocity::apply_velocity(Velocity{ sf::Vector2f{ 10.f, 0.f } }, Transform{ sf::Vector2f{ 0.f, 0.f } }, 1000);
  EXPECT_FLOAT_EQ(result.value.x, 10.f);
  EXPECT_FLOAT_EQ(result.value.y, 0.f);

  auto const result2 =
    velocity::apply_velocity(Velocity{ sf::Vector2f{ -10.f, 0.f } }, Transform{ sf::Vector2f{ 0.f, 0.f } }, 1000);
  EXPECT_FLOAT_EQ(result2.value.x, -10.f);
  EXPECT_FLOAT_EQ(result2.value.y, 0.f);

  auto const result3 =
    velocity::apply_velocity(Velocity{ sf::Vector2f{ 0.f, 10.f } }, Transform{ sf::Vector2f{ 0.f, 0.f } }, 1000);
  EXPECT_FLOAT_EQ(result3.value.x, 0.f);
  EXPECT_FLOAT_EQ(result3.value.y, 10.f);

  auto const result4 =
    velocity::apply_velocity(Velocity{ sf::Vector2f{ 0.f, -10.f } }, Transform{ sf::Vector2f{ 0.f, 0.f } }, 1000);
  EXPECT_FLOAT_EQ(result4.value.x, 0.f);
  EXPECT_FLOAT_EQ(result4.value.y, -10.f);
}
