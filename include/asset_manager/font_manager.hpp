#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace Managers {
class FontManager {
public:
  static FontManager& get_instance() {
    static FontManager instance;
    return instance;
  }

  static TTF_Font* load_font(const char* path, int s) {
    TTF_Font* font = TTF_OpenFont(path, s);
    if (!font) {
      std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
      return nullptr;
    }
    return font;
  }

private:
  FontManager()                              = default;
  FontManager(const FontManager&)            = delete;
  FontManager& operator=(const FontManager&) = delete;
};
} // namespace Managers