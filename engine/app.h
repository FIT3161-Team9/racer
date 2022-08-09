#pragma once

#include <string_view>

/// An app made with this engine.
struct App
{
  /// The name of the app. This is used as the name of the window
  std::string_view const name;
};
