#include "../include/game_context.hpp"

#include "../include/logger.hpp"

namespace Core {

  GameContext::GameContext() = default;

  GameContext::~GameContext() = default;

  GameContext& GameContext::instance() {
    static GameContext instance;
    return instance;
  }

  void GameContext::init(const Core::WindowSpecification& spec) {
    window = Core::Window(spec);
    if (!window.get()) {
      throw std::runtime_error("Failed to initialize window in game context");
    }

    create_renderer(window);
    if (!renderer) {
      throw std::runtime_error("Failed to initialize renderer in game context");
    }

    Vector2 win_dim = window.get_dimension();
    camera          = Camera(
        {0, 0}, win_dim, Camera::LEFT_MARGIN_PCT * win_dim.x, Camera::RIGHT_MARGIN_PCT * win_dim.x);
  }

  void GameContext::create_renderer(Core::Window& window) {
    renderer = SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
      std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
    }
    LOG_INFO("[GameContext] SDL Renderer created");
  }

  void GameContext::set_active_level(LevelData& level) {
    game_data.level_data = level;
  }

  const LevelData& GameContext::get_active_level() const {
    return game_data.level_data;
  }
  const PlayerData& GameContext::get_player_data() const {
    return game_data.player_data;
  }
  const WorldData& GameContext::get_world_data() const {
    return game_data.world_data;
  }

  void GameContext::end() {
    if (renderer) {
      SDL_DestroyRenderer(renderer);
      LOG_INFO("[GameContext] SDL_Renderer destroyed");
      renderer = nullptr;
    }
    LOG_INFO("[GameContext] Ended");
  }
} // namespace Core
