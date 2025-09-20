#include "../../include/asset_manager/texture_manager.hpp"
#include "../../include/asset_manager/utils.hpp"

#include <SDL2/SDL_image.h>

namespace Managers {

TextureManager::TextureManager() {}

TextureManager::~TextureManager() {
  for (auto& pair : textures) {
    SDL_DestroyTexture(pair.second);
  }
}

TextureManager& TextureManager::get_instance() {
  static TextureManager instance;
  return instance;
}

SDL_Texture* TextureManager::get_or_load(const std::string& file, SDL_Renderer* renderer) {
  return TextureManager::get_instance()._get_or_load(asset_path(file), renderer);
}

SDL_Texture* TextureManager::get_texture(const std::string& file) {
  return TextureManager::get_instance()._get_texture(asset_path(file));
}

Vector2 TextureManager::get_texture_dimension(SDL_Texture* texture) {
  int tex_width, tex_height;
  SDL_QueryTexture(texture, nullptr, nullptr, &tex_width, &tex_height);
  return Vector2(tex_width, tex_height);
}

SDL_Texture* TextureManager::_get_or_load(const std::string& file, SDL_Renderer* renderer) {
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

SDL_Texture* TextureManager::_get_texture(const std::string& file) {
  auto it = textures.find(file);
  if (it != textures.end()) {
    return it->second;
  }
  return nullptr;
}

} // namespace Managers
