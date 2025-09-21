#pragma once

#include "../shared.hpp"
#include "asset_manager.hpp"

#include <SDL2/SDL.h>
#include <string>

namespace Managers {

class TextureManagerSingleton : public AssetManager<SDL_Texture*> {
private:
  TextureManagerSingleton();
  ~TextureManagerSingleton();

public:
  TextureManagerSingleton(const TextureManagerSingleton&)            = delete;
  TextureManagerSingleton& operator=(const TextureManagerSingleton&) = delete;

  static TextureManagerSingleton& instance();
  SDL_Texture*                    get_or_load(const std::string& file) override;
  static Vector2                  get_texture_dimension(SDL_Texture* texture);

private:
  SDL_Texture* _get_or_load(const std::string& file);
};

} // namespace Managers
