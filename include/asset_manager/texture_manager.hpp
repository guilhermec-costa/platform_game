#pragma once

#include "../shared.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <unordered_map>

namespace Managers {

class TextureManager {
private:
  std::unordered_map<std::string, SDL_Texture*> textures;

  TextureManager() {}
  ~TextureManager() {
    for (auto& pair : textures) {
      SDL_DestroyTexture(pair.second);
    }
  }

public:
  TextureManager(const TextureManager&)            = delete;
  TextureManager& operator=(const TextureManager&) = delete;

  static TextureManager& get_instance() {
    static TextureManager instance;
    return instance;
  }

  static SDL_Texture* get_or_load(const std::string& file, SDL_Renderer* renderer) {
    return TextureManager::get_instance()._get_or_load(file, renderer);
  }

  static SDL_Texture* get_texture(const std::string& file) {
    return TextureManager::get_instance()._get_texture(file);
  }

  static Vector2 get_texture_dimension(SDL_Texture* texture) {
    int tex_width, tex_height;
    SDL_QueryTexture(texture, nullptr, nullptr, &tex_width, &tex_height);
    return Vector2(tex_width, tex_height);
  }

private:
  SDL_Texture* _get_or_load(const std::string& file, SDL_Renderer* renderer) {
    auto it = textures.find(file);
    if (it != textures.end()) {
      return it->second;
    }

    SDL_Surface* surface = IMG_Load(file.c_str());
    if (!surface) {
      SDL_Log("❌ Failed to load image %s: %s", file.c_str(), IMG_GetError());
      return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
      SDL_Log("❌ Failed to create texture from %s: %s", file.c_str(), SDL_GetError());
      return nullptr;
    }

    textures[file] = texture;
    return texture;
  }

  SDL_Texture* _get_texture(const std::string& file) {
    auto it = textures.find(file);
    if (it != textures.end()) {
      return it->second;
    }
    return nullptr;
  }
};
} // namespace Managers
