#pragma once

#include "../include/shared.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

class SDLBackend {
public:
  static SDL_Window*   create_window(int width, int height);
  static SDL_Renderer* create_renderer(SDL_Window* window);
  static int           init_subsytems();
  static Vector2       get_window_dimension(SDL_Window* window);
  static int           get_window_width(SDL_Window* window);
  static int           get_window_height(SDL_Window* window);
  static Vector2       get_texture_dimensions(SDL_Texture* texture);
  static TTF_Font*     load_font(const char* path, int s);
};