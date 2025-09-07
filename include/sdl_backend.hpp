#pragma once

#include "../include/shared.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

class SDLBackend {
public:
  static SDL_Window*   create_window(int width, int height);
  static SDL_Renderer* create_renderer(SDL_Window* window);
  static int           init_subsytems();
  static Vector2D      get_window_dimension(SDL_Window* window);
};