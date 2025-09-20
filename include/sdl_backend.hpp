#pragma once

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

class SDLBackend {
public:
  static SDL_Renderer* create_renderer(SDL_Window* window);
  static int           init_subsytems();
  static TTF_Font*     load_font(const char* path, int s);
};