#include "../game_context.hpp"
#include "ui_element.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

namespace UI {
class Label : public UIElement {
public:
  Label(const std::string& text, TTF_Font* font, Vector2 pos, Vector2 dim)
      : content(text), font(font) {
    set_position(pos);
    set_dimension(dim);
    update_texture();
  };
  Label(const std::string& text, TTF_Font* font) : content(text), font(font) { update_texture(); }

  void render(SDL_Renderer* renderer) override {
    if (!font)
      return;
    SDL_Color color = {255, 255, 255, 255};
    SDL_Rect  dst{rect.x, rect.y, 100, 100};
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
  }

  void update(float dt) override {}

  void handle_event(const SDL_Event& event) override {}

  ~Label() {
    if (texture) {
      SDL_DestroyTexture(texture);
    }
  }

  void set_text(const std::string& text) {
    if (text == content)
      return;
    content = text;
    update_texture();
  }

  void update_texture() {
    if (texture) {
      SDL_DestroyTexture(texture);
      texture = nullptr;
    }

    if (!font || content.empty())
      return;

    SDL_Color    color   = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, content.c_str(), color);
    texture              = SDL_CreateTextureFromSurface(GameContext::instance().renderer, surface);

    rect.w = surface->w;
    rect.h = surface->h;

    SDL_FreeSurface(surface);
  }

private:
  std::string  content;
  TTF_Font*    font    = nullptr;
  SDL_Texture* texture = nullptr;
};
} // namespace UI