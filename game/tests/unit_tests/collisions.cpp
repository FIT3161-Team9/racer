#pragma once

#include <SFML/Graphics.hpp>
#include <gtest/gtest.h>


#include "engine/circle.h"
#include "engine/transform.h"
#include "game/src/collisions.h"
#include "game/src/segment.h"


TEST(Collisions, PointCircle)
{
  auto const result = collisions::point_circle(
    Circle{ .radius = 10.f }, Transform{ .value = sf::Vector2f{ 0.f, 0.f } }, sf::Vector2f{ 10.f, 0.f });
  EXPECT_TRUE(result);
}

TEST(Collisions, PointSegment)
{
  auto const result = collisions::point_segment(
    Segment{ .start = sf::Vector2f{ 0.f, 0.f }, .end = sf::Vector2f{ 1.f, 1.f } }, sf::Vector2f{ 0.5f, 0.5f });
  EXPECT_TRUE(result);
}

TEST(Collisions, CircleSegment)
{
  {
    auto const result =
      collisions::segment_circle(Segment{ .start = sf::Vector2f{ 1.f, 0.f }, .end = sf::Vector2f{ 5.f, 0.f } },
                                 Circle{ .radius = 1.f },
                                 Transform{ .value = sf::Vector2f{ 0.f, 0.f } });
    EXPECT_TRUE(result);
  }
  {
    auto const result =
      collisions::segment_circle(Segment{ .start = sf::Vector2f{ 1.1f, 0.f }, .end = sf::Vector2f{ 5.f, 0.f } },
                                 Circle{ .radius = 1.f },
                                 Transform{ .value = sf::Vector2f{ 0.f, 0.f } });
    EXPECT_FALSE(result);
  }
  {
    auto const result =
      collisions::segment_circle(Segment{ .start = sf::Vector2f{ -10.f, -5.f }, .end = sf::Vector2f{ 10.f, -5.f } },
                                 Circle{ .radius = 5.f },
                                 Transform{ .value = sf::Vector2f{ 0.f, 0.f } });
    EXPECT_TRUE(result);
  }
  {
    auto const result =
      collisions::segment_circle(Segment{ .start = sf::Vector2f{ -10.f, -5.f }, .end = sf::Vector2f{ 10.f, -5.f } },
                                 Circle{ .radius = 4.9f },
                                 Transform{ .value = sf::Vector2f{ 0.f, 0.f } });
    EXPECT_FALSE(result);
  }
}
