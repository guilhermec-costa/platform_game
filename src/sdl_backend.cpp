#include "../include/sdl_backend.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <stdexcept>

SDL_Window* SDLBackend::create_window(int width, int height) {
  auto window = SDL_CreateWindow("Platform game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 width, height, SDL_WINDOW_RESIZABLE);
  if (!window) {
    std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
    throw std::runtime_error("Failed to create window");
  }
  return window;
}

SDL_Renderer* SDLBackend::create_renderer(SDL_Window* window) {
  if (!window) {
    throw std::runtime_error("Window not initialized");
  }
  auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
    throw std::runtime_error("Failed to create renderer");
  }
  return renderer;
}

int SDLBackend::init_subsytems() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return -1;
  }
  if (TTF_Init() < 0) {
    return -1;
  }
  return 0;
}