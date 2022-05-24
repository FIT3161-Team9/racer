/**
 * Class for internal use by the engine. Basically a cache for loaded resources
 * (Shaders, Textures, etc.)
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <string>

#include "texture.h"

// Forward declare for friend
namespace engine
{
template<typename AppState, typename StartupFn>
void run_app(App const&, StartupFn);
}

class RenderContext
{
  using FilePath = std::string;
  using TextureCache = std::map<FilePath, sf::Texture>;
  using ShaderCache = std::map<FilePath, sf::Shader>;
  TextureCache m_texture_cache{};
  ShaderCache m_shader_cache{};

  template<typename AppState, typename StartupFn>
  friend void engine::run_app(App const&, StartupFn);

  RenderContext() {}

public:
  sf::Texture& get_or_load_texture(Texture const& texture)
  {
    if (!m_texture_cache.contains(texture.path)) {
      sf::Texture sf_texture{};
      if (!sf_texture.loadFromFile(texture.path)) {
        // TODO: Handle texture loading errors
      }
      m_texture_cache.insert({ texture.path, sf_texture });
    }
    return m_texture_cache.at(texture.path);
  }
};
