#pragma once

#include "camera.hpp"
#include "level.hpp"
#include "sdl_backend.hpp"
#include "ui/ui_manager.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

class GameContext {
public:
  SDL_Window*   window   = nullptr;
  SDL_Renderer* renderer = nullptr;
  TTF_Font*     font     = nullptr;
  UI::UIManager ui_manager;
  Camera        camera;

public:
  static GameContext& instance() {
    static GameContext instance;
    return instance;
  }

  void                                  end();
  LevelMetadata::Player&                get_player_data();
  LevelMetadata::World&                 get_world_data();
  std::vector<LevelMetadata::Platform>& get_platforms_data();
  void                                  set_level(const Level& level);
  void                                  set_font(TTF_Font* f);
  void                                  init(SDL_Window* win, SDL_Renderer* rend) {
    Vector2 win_dim = SDLBackend::get_window_dimension(win);
    window          = win;
    renderer        = rend;
    camera = Camera({0, 0}, win_dim, Camera::LEFT_MARGIN_PCT * win_dim.x,
                                                     Camera::RIGHT_MARGIN_PCT * win_dim.x);
  }

  Level& get_level() { return active_level; }

private:
  Level active_level;

  GameContext()                              = default;
  GameContext(const GameContext&)            = delete;
  ~GameContext()                             = default;
  GameContext& operator=(const GameContext&) = delete;
};