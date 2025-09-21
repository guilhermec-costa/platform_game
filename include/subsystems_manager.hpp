#pragma once

#include <iostream>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class SubsystemsManager {
public:
  static void init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
      throw std::runtime_error("[SDL] Video init failed: " + std::string(SDL_GetError()));
    std::cout << "[SDL] Video initialized\n";

    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG)))
      throw std::runtime_error("[SDL] Image init failed: " + std::string(IMG_GetError()));
    std::cout << "[SDL] Image initialized\n";

    if (TTF_Init() < 0)
      throw std::runtime_error("[SDL] TTF init failed: " + std::string(TTF_GetError()));
    std::cout << "[SDL] TTF initialized\n";
  }

  static void shutdown() {
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
  }
};
