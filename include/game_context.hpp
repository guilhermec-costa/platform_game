#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "camera.hpp"
#include "level.hpp"
#include "window.hpp"

namespace Core {

  class GameContext {
  public:
    Core::Window  window;
    SDL_Renderer* renderer = nullptr;
    GameData      game_data;
    Camera        camera;

    static GameContext& instance();

    void        end();
    PlayerData& get_player_data();
    WorldData&  get_world_data();
    LevelData&  get_level_data();
    void        set_level(const LevelData& level);
    void        init(const Core::WindowSpecification& spec);
    LevelData&  get_level();
    void        create_renderer(Core::Window& window);

  private:
    LevelData active_level;

    GameContext();
    GameContext(const GameContext&) = delete;
    ~GameContext();
    GameContext& operator=(const GameContext&) = delete;
  };
} // namespace Core
