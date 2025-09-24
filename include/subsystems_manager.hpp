#pragma once

#include <iostream>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "asset_manager/audio_manager.hpp"
#include "logger.hpp"

class SubsystemsManager {
public:
  static void init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
      throw std::runtime_error("[SDL] Video init failed: " + std::string(SDL_GetError()));
    LOG_INFO("[SDL] Video initialized");

    Managers::AudioManager::instance().init();

    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG)))
      throw std::runtime_error("[SDL] Image init failed: " + std::string(IMG_GetError()));
    LOG_INFO("[SDL] Image initialized");

    if (TTF_Init() < 0)
      throw std::runtime_error("[SDL] TTF init failed: " + std::string(TTF_GetError()));
    LOG_INFO("[SDL] TTF initialized");
  }

  static void shutdown() {
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
  }
};
