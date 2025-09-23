#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "asset_manager/audio_manager.hpp"
#include "camera.hpp"
#include "ground.hpp"
#include "level.hpp"
#include "window.hpp"

namespace Core {

  class GameContext {
  public:
    Core::Window            window;
    SDL_Renderer*           renderer = nullptr;
    Camera                  camera;
    GameData                game_data;
    Managers::AudioManager& audio_manager = Managers::AudioManager::instance();
    Ground                  global_ground;

    static GameContext& instance();

    void end();
    void init(const Core::WindowSpecification& spec);
    void create_renderer(Core::Window& window);

    void        set_active_level(LevelData& level);
    void        set_player_data(PlayerData& player);
    void        set_world_data(WorldData& world);
    LevelData&  get_active_level();
    PlayerData& get_player_data();
    WorldData&  get_world_data();
    GameData&   get_game_data();

    GameContext();
    ~GameContext();

    GameContext(const GameContext&)            = delete;
    GameContext& operator=(const GameContext&) = delete;
  };
} // namespace Core
