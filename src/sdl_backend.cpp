#include "../include/sdl_backend.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <stdexcept>

SDL_Window* SDLBackend::create_window(int width, int height) {
  auto window = SDL_CreateWindow("Platform game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 width, height, 0);
  if (!window) {
    std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
    throw std::runtime_error("Failed to create window");
  }
  std::cout << "[SDLBackend] SDL Window created\n";
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
  std::cout << "[SDLBackend] SDL Renderer created\n";
  return renderer;
}

int SDLBackend::init_subsytems() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "[SDLBackend] SDL Video subystem initialization failed\n";
    return -1;
  }
  std::cout << "[SDLBackend] SDL Video subystem initialized\n";
  if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)) {
    std::cout << "[SDLBackend] SDL Image subystem initialization failed\n";
    return -1;
  }
  std::cout << "[SDLBackend] SDL Image subystem initialized\n";
  if (TTF_Init() < 0) {
    std::cout << "[SDLBackend] SDL TTF subystem initialization failed\n";
    return -1;
  }
  std::cout << "[SDLBackend] SDL TTF subystem initialized\n";
  std::cout << "[SDLBackend] Finished initializing SDL subsystems\n";
  return 0;
}

Vector2 SDLBackend::get_window_dimension(SDL_Window* window) {
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  return Vector2(w, h);
}

int SDLBackend::get_window_width(SDL_Window* window) {
  return get_window_dimension(window).x;
}

int SDLBackend::get_window_height(SDL_Window* window) {
  return get_window_dimension(window).y;
}

Vector2 SDLBackend::get_texture_dimensions(SDL_Texture* texture) {
  int tex_width, tex_height;
  SDL_QueryTexture(texture, nullptr, nullptr, &tex_width, &tex_height);
  return Vector2(tex_width, tex_height);
}

TTF_Font* SDLBackend::load_font(const char* path, int s) {
  TTF_Font* font = TTF_OpenFont(path, s);
  if (!font) {
    std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    return nullptr;
  }
  return font;
}