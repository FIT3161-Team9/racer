/**
 * Class for internal use by the engine. Basically a cache for loaded resources
 * (Shaders, Textures, etc.)
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <iostream>
#include <map>
#include <string>

#include "app.h"
#include "text.h"
#include "texture.h"

// Forward declare for friend
namespace engine
{
template<typename StartupFn>
void run_app(App const&, StartupFn);
}

class RenderContext
{
  using FilePath = std::string;
  using FontCache = std::map<FilePath, sf::Font>;
  using TextureCache = std::map<FilePath, sf::Texture>;
  using ShaderCache = std::map<FilePath, sf::Shader>;
  TextureCache m_texture_cache{};
  FontCache m_font_cache{};
  ShaderCache m_shader_cache{};
  entt::registry& m_entity_registry;

  template<typename StartupFn>
  friend void engine::run_app(App const&, StartupFn);

  RenderContext(entt::registry& entity_registry) : m_entity_registry(entity_registry) {}

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

  sf::Font& get_or_load_font(Text const& text)
  {
    if (!m_font_cache.contains(text.font)) {
      sf::Font sf_font{};
      if (!sf_font.loadFromFile(text.font)) {
        // TODO: Handle font loading errors
      }
      m_font_cache.insert({ text.font, sf_font });
    }
    return m_font_cache.at(text.font);
  }

  template<typename ComponentType>
  ComponentType* get_component(entt::entity entity)
  {
    return m_entity_registry.try_get<ComponentType>(entity);
  }
};
