#pragma once

#include "ui_element.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

namespace UI {

class Label : public UIElement {
public:
  Label(const std::string& text, TTF_Font* font, Vector2 pos, Vector2 dim);
  Label(const std::string& text, TTF_Font* font);

  ~Label() override;

  void render(SDL_Renderer* renderer) override;
  void update(float dt) override;
  void handle_event(const SDL_Event& event) override;

  void set_text(const std::string& text);
  void update_texture();

private:
  std::string  content;
  TTF_Font*    font    = nullptr;
  SDL_Texture* texture = nullptr;
};

} // namespace UI
