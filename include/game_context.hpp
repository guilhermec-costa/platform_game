#pragma once

#include "camera.hpp"
#include "level.hpp"
#include "ui/ui_manager.hpp"
#include "window.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

class GameContext {
public:
  Core::Window  window;
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
  void                                  init(const Core::WindowSpecification& spec) {
    window = Core::Window(spec);
    if (!window.get()) {
      throw std::runtime_error("Failed to initialize window in game context");
    }

    create_renderer(window);
    if (!renderer) {
      throw std::runtime_error("Failed to initialize renderer in game context");
    }

    Vector2 win_dim = window.get_dimension();
    camera = Camera({0, 0}, win_dim, Camera::LEFT_MARGIN_PCT * win_dim.x,
                                                     Camera::RIGHT_MARGIN_PCT * win_dim.x);
  }

  Level& get_level() { return active_level; }

  void create_renderer(Core::Window& window) {
    renderer = SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
      std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
    }
    std::cout << "[SDLBackend] SDL Renderer created\n";
  }

private:
  Level active_level;

  GameContext()                              = default;
  GameContext(const GameContext&)            = delete;
  ~GameContext()                             = default;
  GameContext& operator=(const GameContext&) = delete;
};