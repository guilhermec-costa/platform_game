#include "../include/game_context.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

void GameContext::end() {
  SDL_DestroyRenderer(renderer);
  std::cout << "[GameContext] SDL Renderer destroyed\n";
  SDL_DestroyWindow(window);
  std::cout << "[GameContext] SDL Window destroyed\n";
  std::cout << "[GameContext] GameContext ended\n";
}

GameContext::GameContext(SDL_Window* window, SDL_Renderer* renderer)
    : window(window), renderer(renderer) {
  auto win_dim = SDLBackend::get_window_dimension(window);
  camera       = Camera({0, 0}, win_dim, Camera::LEFT_MARGIN_PCT * win_dim.x,
                        Camera::RIGHT_MARGIN_PCT * win_dim.x);
  std::cout << "[GameContext] GameContext initialized with window and renderer\n";
};

LevelMetadata::Player& GameContext::get_player_data() {
  return active_level.player;
}
std::vector<LevelMetadata::Platform>& GameContext::get_platforms_data() {
  return active_level.platforms;
}

LevelMetadata::World& GameContext::get_world_data() {
  return active_level.world;
}

void GameContext::set_level(const Level& level) {
  active_level = level;
}