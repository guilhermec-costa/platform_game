#include "../../include/asset_manager/texture_manager.hpp"

#include "../../include/asset_manager/utils.hpp"
#include "../../include/game_context.hpp"

#include <SDL2/SDL_image.h>

namespace Managers {

TextureManagerSingleton::TextureManagerSingleton() : AssetManager<SDL_Texture*>() {}

TextureManagerSingleton::~TextureManagerSingleton() {
  for (const auto& pair : m_assets) {
    SDL_DestroyTexture(pair.second);
  }
}

TextureManagerSingleton& TextureManagerSingleton::instance() {
  static TextureManagerSingleton instance;
  return instance;
}

SDL_Texture* TextureManagerSingleton::get_or_load(const std::string& file) {
  return TextureManagerSingleton::instance()._get_or_load(asset_path(file));
}

Vector2 TextureManagerSingleton::get_texture_dimension(SDL_Texture* texture) {
  int tex_width, tex_height;
  SDL_QueryTexture(texture, nullptr, nullptr, &tex_width, &tex_height);
  return Vector2(tex_width, tex_height);
}

SDL_Texture* TextureManagerSingleton::_get_or_load(const std::string& file) {
  SDL_Texture* tex = get_asset(file);
  if (tex)
    return tex;

  SDL_Surface* surface = IMG_Load(file.c_str());
  if (!surface) {
    SDL_Log("❌ Failed to load image %s: %s", file.c_str(), IMG_GetError());
    return nullptr;
  }

  SDL_Renderer* renderer = Core::GameContext::instance().renderer;
  SDL_Texture*  texture  = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  if (!texture) {
    SDL_Log("❌ Failed to create texture from %s: %s", file.c_str(), SDL_GetError());
    return nullptr;
  }

  m_assets[file] = texture;
  return texture;
}
} // namespace Managers
