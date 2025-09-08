#pragma once

#include "../camera.hpp"
#include "../shared.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

class TextureComponent {
public:
  TextureComponent() : m_texture(nullptr) {}

  TextureComponent(SDL_Texture* texture, Vector2D pos, Vector2D dim) : m_texture(texture) {
    src_rect    = {0, 0, (int)dim.x, (int)dim.y};
    target_rect = {(int)pos.x, (int)pos.y, (int)dim.x, (int)dim.y};
  }

  void render(SDL_Renderer* renderer, const Camera& camera) {
    if (!m_texture)
      return;

    SDL_Rect screen_rect = target_rect;
    screen_rect.x -= (int)camera.x;
    screen_rect.y -= (int)camera.y;

    SDL_RenderCopy(renderer, m_texture, &src_rect, &screen_rect);
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

  void rotate(double angle, SDL_Renderer* renderer, const Camera& camera) {
    if (!m_texture)
      return;

    SDL_Rect screen_rect = target_rect;
    screen_rect.x -= (int)camera.x;
    screen_rect.y -= (int)camera.y;

    SDL_RenderCopyEx(renderer, m_texture, &src_rect, &screen_rect, angle, nullptr, SDL_FLIP_NONE);
  }

private:
  SDL_Texture* m_texture;
  SDL_Rect     src_rect;
  SDL_Rect     target_rect;
};
