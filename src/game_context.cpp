#include "../include/game_context.hpp"

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
    std::cout << "[GameContext] SDL Renderer created\n";
  }

  Level& GameContext::get_level() {
    return active_level;
  }

  LevelMetadata::Player& GameContext::get_player_data() {
    return active_level.player;
  }

  LevelMetadata::World& GameContext::get_world_data() {
    return active_level.world;
  }

  std::vector<LevelMetadata::Platform>& GameContext::get_platforms_data() {
    return active_level.platforms;
  }

  void GameContext::set_level(const Level& level) {
    active_level = level;
  }

  void GameContext::set_font(TTF_Font* f) {
    font = f;
  }

  void GameContext::end() {
    if (renderer) {
      SDL_DestroyRenderer(renderer);
      renderer = nullptr;
    }
    std::cout << "[GameContext] Ended\n";
  }
} // namespace Core
