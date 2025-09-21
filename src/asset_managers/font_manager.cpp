#include "../../include/asset_manager/font_manager.hpp"

#include <iostream>

#include <SDL2/SDL_ttf.h>

namespace Managers {

  FontManagerSingleton::FontManagerSingleton() : AssetManager<TTF_Font*>() {}

  FontManagerSingleton::~FontManagerSingleton() {
    for (const auto& p : m_assets) {
      if (p.second)
        TTF_CloseFont(p.second);
    }
  }

  TTF_Font* FontManagerSingleton::get_or_load(const std::string& path) {
    const std::string normalized_path = asset_path(path);
    TTF_Font*         font            = TTF_OpenFont(normalized_path.c_str(), 14);
    if (!font) {
      std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
      return nullptr;
    }
    m_assets[normalized_path] = font;
    return font;
  }
} // namespace Managers