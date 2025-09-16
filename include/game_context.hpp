#pragma once

#include "camera.hpp"
#include "sdl_backend.hpp"
#include "level.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

class GameContext {
public:
  SDL_Window*   window   = nullptr;
  SDL_Renderer* renderer = nullptr;
  TTF_Font*     font     = nullptr;
  Camera        camera;
  Level active_level;

public:
  GameContext()             = default;
  GameContext(GameContext&) = delete;
  GameContext(SDL_Window* window, SDL_Renderer* renderer) : window(window), renderer(renderer) {
    auto win_dim = SDLBackend::get_window_dimension(window);
    camera       = Camera({0, 0}, win_dim, Camera::LEFT_MARGIN_PCT * win_dim.x,
                          Camera::RIGHT_MARGIN_PCT * win_dim.x);
    std::cout << "[GameContext] GameContext initialized with window and renderer\n";
  };
  void end();
};