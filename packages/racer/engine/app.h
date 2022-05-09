#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "entity.h"

struct App
{
  std::string_view const name;
  std::vector<std::shared_ptr<Entity>> entities;
};
