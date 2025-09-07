#pragma once

#include <SDL2/SDL_render.h>

class BackgroundParallax {
public:
  BackgroundParallax()                          = default;
  BackgroundParallax(const BackgroundParallax&) = delete;
  BackgroundParallax(SDL_Renderer* renderer);
  void update(float camera_x);
  void render(SDL_Window* window, SDL_Renderer* renderer);

private:
  SDL_Texture* bg_texture;
  SDL_Texture* far_trees_texture;
  SDL_Texture* mid_trees_texture;
  SDL_Texture* trees_texture;
  float        bg_x, mid_x, trees_x;
};