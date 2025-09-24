#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "../shared.hpp"
#include "asset_manager.hpp"

namespace Managers {

  class TextureManagerSingleton : public AssetManager<SDL_Texture*> {
  public:
    ~TextureManagerSingleton();

    static TextureManagerSingleton& instance();
    SDL_Texture*                    get_or_load(const std::string& file) override;
    static Vector2                  get_texture_dimension(SDL_Texture* texture);

  private:
    TextureManagerSingleton(const TextureManagerSingleton&)            = delete;
    TextureManagerSingleton& operator=(const TextureManagerSingleton&) = delete;
    TextureManagerSingleton();
  };

} // namespace Managers

using TextureManager = Managers::TextureManagerSingleton;