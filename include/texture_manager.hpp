#pragma once

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <string>
enum class TextureID { Grass, Player, Enemy };

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <unordered_map>

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

  SDL_Texture* load_texture(const std::string& file, SDL_Renderer* renderer) {
    if (textures.count(file)) {
      return textures[file];
    }

    SDL_Surface* tempSurface = IMG_Load(file.c_str());
    if (!tempSurface) {
      SDL_Log("Failed to load image %s: %s", file.c_str(), IMG_GetError());
      return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (!texture) {
      SDL_Log("Failed to create texture from %s: %s", file.c_str(), SDL_GetError());
      return nullptr;
    }

    textures[file] = texture;
    return texture;
  }

  SDL_Texture* get_texture(const std::string& file) {
    if (textures.count(file))
      return textures[file];
    return nullptr;
  }

  static inline std::string asset_path(const std::string& relative) {
    return std::string(PROJECT_SOURCE_DIR) + relative;
  }
};
