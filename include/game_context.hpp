#pragma once

#include "camera.hpp"
#include "level.hpp"
#include "sdl_backend.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

class GameContext {
public:
  SDL_Window*   window   = nullptr;
  SDL_Renderer* renderer = nullptr;
  TTF_Font*     font     = nullptr;
  Camera        camera;

public:
  GameContext()             = default;
  GameContext(GameContext&) = delete;
  GameContext(SDL_Window* window, SDL_Renderer* renderer);
  void                                  end();
  LevelMetadata::Player&                get_player_data();
  LevelMetadata::World&                 get_world_data();
  std::vector<LevelMetadata::Platform>& get_platforms_data();
  void                                  set_level(const Level& level);

private:
  Level active_level;
};