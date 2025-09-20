#pragma once

#include "camera.hpp"
#include "level.hpp"
#include "ui/ui_manager.hpp"
#include "window.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Core {

class GameContext {
public:
  Core::Window  window;
  SDL_Renderer* renderer = nullptr;
  TTF_Font*     font     = nullptr;
  UI::UIManager ui_manager;
  Camera        camera;

  static GameContext& instance();

  void                                  end();
  LevelMetadata::Player&                get_player_data();
  LevelMetadata::World&                 get_world_data();
  std::vector<LevelMetadata::Platform>& get_platforms_data();
  void                                  set_level(const Level& level);
  void                                  set_font(TTF_Font* f);
  void                                  init(const Core::WindowSpecification& spec);
  Level&                                get_level();
  void                                  create_renderer(Core::Window& window);

private:
  Level active_level;

  GameContext();
  GameContext(const GameContext&) = delete;
  ~GameContext();
  GameContext& operator=(const GameContext&) = delete;
};
} // namespace Core
