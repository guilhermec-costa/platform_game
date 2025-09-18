#include "../game_context.hpp"
#include "ui_element.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

class Label : public UIElement {
public:
  Label(const std::string& text, TTF_Font* font) : content(text), font(font) {
    SDL_Color    color   = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, content.c_str(), color);
    texture              = SDL_CreateTextureFromSurface(GameContext::instance().renderer, surface);
    SDL_FreeSurface(surface);
  }

  void render(SDL_Renderer* renderer) override {
    if (!font)
      return;
    SDL_Color color = {255, 255, 255, 255};
    SDL_Rect  dst{rect.x, rect.y, 100, 100};
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
  }

  void update(float dt) override {}

  void handle_event(const SDL_Event& event) override {}

  ~Label() { SDL_DestroyTexture(texture); }

private:
  std::string  content;
  TTF_Font*    font    = nullptr;
  SDL_Texture* texture = nullptr;
  SDL_Color    color;
};