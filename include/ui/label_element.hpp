#include "ui_element.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

class Label : public UIElement {
public:
  Label(const std::string& text, TTF_Font* font) : content(text), font(font) {}

  void render(SDL_Renderer* renderer) override {
    if (!font)
      return;
    SDL_Color    color   = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, content.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect     dst{rect.x, rect.y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
  }

  void update(float dt) override {}

  void handle_event(const SDL_Event& event) override {}

private:
  std::string content;
  TTF_Font*   font = nullptr;
};