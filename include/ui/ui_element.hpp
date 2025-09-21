#pragma once

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "../shared.hpp"

namespace UI {
  class UIElement {
  public:
    virtual ~UIElement() = default;

    virtual void render(SDL_Renderer* renderer)   = 0;
    virtual void handle_event(const SDL_Event& e) = 0;
    virtual void update(float dt)                 = 0;

    void set_position(Vector2 pos) {
      rect.x = pos.x;
      rect.y = pos.y;
    }

    void set_dimension(Vector2 dim) {
      rect.w = dim.x;
      rect.h = dim.y;
    }

    SDL_Rect get_rect() const {
      return rect;
    }

  protected:
    SDL_Rect rect{0, 0, 0, 0};
  };
} // namespace UI