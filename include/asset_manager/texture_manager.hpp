#pragma once

#include "../shared.hpp"

#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

namespace Managers {

class TextureManager {
private:
  std::unordered_map<std::string, SDL_Texture*> textures;

  TextureManager();
  ~TextureManager();

public:
  TextureManager(const TextureManager&)            = delete;
  TextureManager& operator=(const TextureManager&) = delete;

  static TextureManager& get_instance();

  static SDL_Texture* get_or_load(const std::string& file, SDL_Renderer* renderer);
  static SDL_Texture* get_texture(const std::string& file);
  static Vector2      get_texture_dimension(SDL_Texture* texture);

private:
  SDL_Texture* _get_or_load(const std::string& file, SDL_Renderer* renderer);
  SDL_Texture* _get_texture(const std::string& file);
};

} // namespace Managers
