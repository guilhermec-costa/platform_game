#include "../../include/ui/label_element.hpp"

#include "../../include/game_context.hpp"

namespace UI {

Label::Label(const std::string& text, TTF_Font* font, Vector2 pos, Vector2 dim)
    : content(text), font(font) {
  set_position(pos);
  set_dimension(dim);
  update_texture();
}

Label::Label(const std::string& text, TTF_Font* font) : content(text), font(font) {
  update_texture();
}

Label::~Label() {
  if (texture) {
    SDL_DestroyTexture(texture);
  }
}

void Label::render(SDL_Renderer* renderer) {
  if (!font || !texture)
    return;

  SDL_Rect dst{rect.x, rect.y, rect.w, rect.h};
  SDL_RenderCopy(renderer, texture, nullptr, &dst);
}

void Label::update(float dt) {}

void Label::handle_event(const SDL_Event& event) {}

void Label::set_text(const std::string& text) {
  if (text == content)
    return;
  content = text;
  update_texture();
}

void Label::update_texture() {
  if (texture) {
    SDL_DestroyTexture(texture);
    texture = nullptr;
  }

  if (!font || content.empty())
    return;

  SDL_Renderer* renderer = Core::GameContext::instance().renderer;
  SDL_Color     color    = {255, 255, 255, 255};
  SDL_Surface*  surface  = TTF_RenderText_Solid(font, content.c_str(), color);
  texture                = SDL_CreateTextureFromSurface(renderer, surface);

  rect.w = surface->w;
  rect.h = surface->h;

  SDL_FreeSurface(surface);
}

} // namespace UI
