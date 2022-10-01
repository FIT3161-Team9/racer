#pragma once

#include <SFML/Graphics.hpp>

#include <engine/rectangle.h>
#include <engine/rotation.h>
#include <engine/transform.h>
#include <engine/vector_utils.h>

#include "../src/segment.h"

TEST(RectangleUtils, Segments)
{
  auto const result = rectangle_utils::segments(Rectangle{ .width_height = sf::Vector2f{ 2.f, 2.f } },
                                                Transform{ .value = sf::Vector2f{ 0.f, 0.f } },
                                                Rotation{ .degrees = 90.f });
  EXPECT_EQ(result.size(), 4);

  auto const error_threshold = 0.05;

  EXPECT_NEAR(result[3].start.x, -1.f, error_threshold);
  EXPECT_NEAR(result[3].start.y, 1.f, error_threshold);
  EXPECT_NEAR(result[3].end.x, 1.f, error_threshold);
  EXPECT_NEAR(result[3].end.y, 1.f, error_threshold);

  EXPECT_NEAR(result[0].start.x, 1.f, error_threshold);
  EXPECT_NEAR(result[0].start.y, 1.f, error_threshold);
  EXPECT_NEAR(result[0].end.x, 1.f, error_threshold);
  EXPECT_NEAR(result[0].end.y, -1.f, error_threshold);

  EXPECT_NEAR(result[1].start.x, 1.f, error_threshold);
  EXPECT_NEAR(result[1].start.y, -1.f, error_threshold);
  EXPECT_NEAR(result[1].end.x, -1.f, error_threshold);
  EXPECT_NEAR(result[1].end.y, -1.f, error_threshold);

  EXPECT_NEAR(result[2].start.x, -1.f, error_threshold);
  EXPECT_NEAR(result[2].start.y, -1.f, error_threshold);
  EXPECT_NEAR(result[2].end.x, -1.f, error_threshold);
  EXPECT_NEAR(result[2].end.y, 1.f, error_threshold);
}
