#include "../include/game.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include "../include/asset_manager/asset_factory_method.hpp"
#include "../include/game_data_manager.hpp"
#include "../include/subsystems_manager.hpp"

namespace Core {

  Game::Game(const GameSpecification& game_spec) : running(true), fps_counter() {
    SubsystemsManager::init();
    GameContext::instance().init(game_spec.window_spec);

    load_global_assets();

    WorldData  world  = GameDataManager::load_world();
    PlayerData player = GameDataManager::load_player(world);
    LevelData  level1 = GameDataManager::load_level("assets/phases/level1.json", world);
    ctx.set_active_level(level1);
    ctx.game_data = GameData{player, world, level1};
  }

  void Game::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit();
      }
      for (const auto& layer : layers) {
        if (!layer->is_suspended()) {
          layer->handle_event(event);
        }
      }
    }
  }

  void Game::update(float dt) {
    for (const auto& layer : layers) {
      if (!layer->is_suspended()) {
        layer->update(dt);
      }
    }
  }

  void Game::render() {
    SDL_SetRenderDrawColor(ctx.renderer, 0, 0, 0, 255);
    SDL_RenderClear(ctx.renderer);
    for (const auto& layer : layers) {
      if (!layer->is_suspended()) {
        layer->render();
      }
    }
    SDL_RenderPresent(ctx.renderer);
  }

  void Game::load_global_assets() {
    const std::string assets[] = {
        "assets/images/nigthborne.png",
        "assets/images/enemy1_spritesheet.png",
        "assets/images/parallax/bg.png",
        "assets/images/parallax/far-trees.png",
        "assets/images/parallax/mid-trees.png",
        "assets/images/parallax/close-trees.png",
        "assets/images/grass.png",
        "assets/fonts/YoungSerif-Regular.ttf",
        "assets/phases/level1.json",
        "assets/phases/level2.json",
        "assets/phases/player_data.json",
        "assets/phases/world_data.json",
        "assets/sounds/walk_grass.wav",
        "assets/sounds/forest_ambience.wav",
    };

    for (const auto& path : assets) {
      if (!AssetFactoryMethod::load(path)) {
        throw std::runtime_error("Unsupported asset type for path" + path);
      }
    }

    ctx.audio_manager.register_channel({.channel = GameAudioChannel::FOREST_AMBIENCE,
                                        .path    = "assets/sounds/forest_ambience.wav",
                                        .volume  = MIX_MAX_VOLUME / 3,
                                        .loop    = true});
    ctx.audio_manager.register_channel({.channel = GameAudioChannel::WALK_ON_GRASS,
                                        .path    = "assets/sounds/walk_grass.wav",
                                        .volume  = MIX_MAX_VOLUME,
                                        .loop    = true});
    ctx.audio_manager.register_channel({.channel = GameAudioChannel::SWORD_MISS_ATTACK,
                                        .path    = "assets/sounds/sword_attack.wav",
                                        .volume  = MIX_MAX_VOLUME,
                                        .loop    = false});
  }

  void Game::run() {
    Uint32    frame_start;
    Uint32    last_time = SDL_GetTicks();
    int       frame_time;
    const int MS_FRAME_DELAY = 1000 / target_fps;

    while (running) {
      frame_start = SDL_GetTicks();
      float dt    = (frame_start - last_time) / 1000.0f;
      last_time   = frame_start;
      handle_events();
      update(dt);
      render();

      frame_time = SDL_GetTicks() - frame_start;
      if (MS_FRAME_DELAY > frame_time) {
        SDL_Delay(MS_FRAME_DELAY - frame_time);
      }
      fps_counter.frame_rendered();
    }
  }

  void Game::quit() {
    LOG_INFO("[Game] Quitting the game");
    running = false;
    ctx.end();
  }

  void Game::push_layer(std::unique_ptr<GameLayer> layer) {
    layers.push_back(std::move(layer));
  }
} // namespace Core