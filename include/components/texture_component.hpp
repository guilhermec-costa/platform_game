#pragma once

#include "../camera.hpp"
#include "../shared.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

class TextureComponent {
public:
  TextureComponent() : m_texture(nullptr) {}

  TextureComponent(SDL_Texture* texture, Vector2D pos, Vector2D dim)
      : m_texture(texture), pos(pos), dim(dim) {
    src_rect    = {0, 0, (int)dim.x, (int)dim.y};
    target_rect = {(int)pos.x, (int)pos.y, (int)dim.x, (int)dim.y};
  }

  void render(SDL_Renderer* renderer, const Camera& camera) {
    if (!m_texture)
      return;

    SDL_RenderCopy(renderer, m_texture, &src_rect, &target_rect);
  }

  void set_position(Vector2D pos) {
    target_rect.x = (int)pos.x;
    target_rect.y = (int)pos.y;
  }

  void set_dimension(Vector2D dim) {
    target_rect.w = dim.x;
    target_rect.h = dim.y;
  }

  void scale(float proportion) {
    target_rect.w = (int)(target_rect.w * proportion);
    target_rect.h = (int)(target_rect.h * proportion);
  }

  const Vector2D& get_pos() const { return pos; };
  const Vector2D& get_dimension() const { return dim; };
  SDL_Texture*    get_texture() { return m_texture; }

private:
  SDL_Texture* m_texture;
  SDL_Rect     src_rect;
  SDL_Rect     target_rect;
  Vector2D     pos;
  Vector2D     dim;
};
