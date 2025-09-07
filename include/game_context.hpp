#pragma once

#include "sdl_backend.hpp"
#include "camera.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

class GameContext {
public:
  SDL_Window*   window   = nullptr;
  SDL_Renderer* renderer = nullptr;
  TTF_Font*     font     = nullptr;
  Camera        camera;

public:
  GameContext()             = default;
  GameContext(GameContext&) = delete;
  GameContext(SDL_Window* window, SDL_Renderer* renderer) : window(window), renderer(renderer) {
    auto win_dim = SDLBackend::get_window_dimension(window);
    camera       = Camera{
              .x = 0,
              .y = 0,
              .w = win_dim.x,
              .h = win_dim.y,
    };

    std::cout << "[GameContext] GameContext initialized with window and renderer\n";
  };
  void end();
};