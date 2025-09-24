#pragma once

#include <SDL2/SDL_ttf.h>

#include "asset_manager.hpp"

namespace Managers {

  class FontManagerSingleton : public AssetManager<TTF_Font*> {
  public:
    ~FontManagerSingleton();

    static FontManagerSingleton& instance() {
      static FontManagerSingleton instance;
      return instance;
    }

    TTF_Font* get_or_load(const std::string& file) override;

  private:
    FontManagerSingleton();
    FontManagerSingleton(const FontManagerSingleton&)            = delete;
    FontManagerSingleton& operator=(const FontManagerSingleton&) = delete;
  };
} // namespace Managers

using FontManager = Managers::FontManagerSingleton;