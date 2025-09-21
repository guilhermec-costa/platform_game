#include "../include/game.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include "../include/asset_manager/asset_factory_method.hpp"
#include "../include/asset_manager/json_manager.hpp"
#include "../include/level.hpp"

namespace Core {

  Game::Game(const GameSpecification& game_spec) : running(true) {
    init_subsytems();
    GameContext::instance().init(game_spec.window_spec);

    load_global_assets();
    load_game_data();
    LevelData l = load_level("assets/phases/level1.json");
    ctx.set_level(l);
    fps_counter = FPSCounter();
  }

  void Game::load_game_data() {
    Vector2 window_dimension = ctx.window.get_dimension();
    json*   player_data_json =
        Managers::JSONManager::instance().get_asset("assets/phases/player_data.json");
    json* world_data_json =
        Managers::JSONManager::instance().get_asset("assets/phases/world_data.json");
    json* level1_data_json =
        Managers::JSONManager::instance().get_asset("assets/phases/level1.json");

    WorldData world_data = WorldData::from_json(*world_data_json);
    LevelData level_data =
        LevelData::from_json(*level1_data_json, window_dimension.y, world_data.max_horizontal_x);
    PlayerData player_data =
        PlayerData::from_json(*player_data_json, window_dimension.y, world_data.max_horizontal_x);

    ctx.game_data = GameData{player_data, world_data, level_data};
  };

  void Game::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit();
      }
      for (const auto& layer : layers) {
        layer->handle_event(event);
      }
    }
  }

  void Game::update(float dt) {
    for (const auto& layer : layers) {
      layer->update(dt);
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
    };

    for (const auto& path : assets) {
      if (!AssetFactoryMethod::load(path)) {
        throw std::runtime_error("Unsupported asset type for path" + path);
      }
    }
  }

  LevelData Game::load_level(const std::string& level_name) {
    int       window_height = ctx.window.get_height();
    json*     j             = Managers::JSONManager::instance().get_asset(level_name);
    LevelData level =
        LevelData::from_json(*j, window_height, ctx.get_world_data().max_horizontal_x);
    return level;
  }

  void Game::init_subsytems() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      throw std::runtime_error("[Game] SDL Video subsystem initialization failed: " +
                               std::string(SDL_GetError()));
    }
    std::cout << "[Game] SDL Video subsystem initialized\n";

    if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)) {
      throw std::runtime_error("[Game] SDL Image subsystem initialization failed: " +
                               std::string(IMG_GetError()));
    }
    std::cout << "[Game] SDL Image subsystem initialized\n";

    if (TTF_Init() < 0) {
      throw std::runtime_error("[Game] SDL TTF subsystem initialization failed: " +
                               std::string(TTF_GetError()));
    }
    std::cout << "[Game] SDL TTF subsystem initialized\n";

    std::cout << "[Game] Finished initializing SDL subsystems\n";
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
    running = false;
    std::cout << "[Game] Quitting the game\n";
    ctx.end();
  }

  void Game::push_layer(std::unique_ptr<GameLayer> layer) {
    layers.push_back(std::move(layer));
  }
} // namespace Core