#include "../include/game_context.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

void GameContext::end() {
  if (renderer) {
    SDL_DestroyRenderer(renderer);
    std::cout << "[GameContext] SDL Renderer destroyed\n";
    renderer = nullptr;
  }

  if (window) {
    SDL_DestroyWindow(window);
    std::cout << "[GameContext] SDL Window destroyed\n";
    window = nullptr;
  }

  if (font) {
    TTF_CloseFont(font);
    std::cout << "[GameContext] Font closed\n";
    font = nullptr;
  }

  TTF_Quit();
  std::cout << "[GameContext] TTF quit\n";

  std::cout << "[GameContext] GameContext ended\n";
}

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

void GameContext::set_font(TTF_Font* f) {
  font = f;
}