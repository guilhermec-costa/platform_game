#include "../../include/ui/label_element.hpp"

#include <SDL2/SDL_render.h>

#include "../../include/asset_manager/font_manager.hpp"
#include "../../include/game_context.hpp"
#include "../../include/logger.hpp"

namespace UI {

  Label::Label(const std::string& text, TTF_Font* font, Vector2 pos, Vector2 dim) :
    UIElement(), content(text), font(font) {
    set_position(pos);
    set_dimension(dim);
    update_texture();
  }

  Label::~Label() {
    if (texture) {
      LOG_DEBUG("Destroying texture");
      SDL_DestroyTexture(texture);
    }
  }

  void Label::render(SDL_Renderer* renderer) {
    if (!font || !texture)
      return;

    SDL_RenderDrawRect(renderer, &render_rect);
    SDL_RenderCopy(renderer, texture, nullptr, &render_rect);
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
    SDL_Surface*  surface  = TTF_RenderText_Blended(
        FontManager::instance().get_asset("assets/fonts/YoungSerif-Regular.ttf"),
        content.c_str(),
        color);

    if (!surface) {
      std::cerr << "Failed to render text surface: " << TTF_GetError() << "\n";
      return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
      std::cerr << "Failed to create texture: " << SDL_GetError() << "\n";
      SDL_FreeSurface(surface);
      return;
    }
    render_rect.w = surface->w;
    render_rect.h = surface->h;
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(surface);
  }

} // namespace UI
