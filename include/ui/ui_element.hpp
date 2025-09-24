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
      render_rect.x = pos.x;
      render_rect.y = pos.y;
    }

    void set_dimension(Vector2 dim) {
      render_rect.w = dim.x;
      render_rect.h = dim.y;
    }

    SDL_Rect get_rect() const {
      return render_rect;
    }

  protected:
    SDL_Rect render_rect{0, 0, 0, 0};
  };
} // namespace UI