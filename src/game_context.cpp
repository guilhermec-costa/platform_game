#include "../include/game_context.hpp"
#include "../include/sdl_backend.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

void GameContext::end() {
  SDL_DestroyRenderer(m_renderer);
  std::cout << "[GameContext] SDL Renderer destroyed\n";
  SDL_DestroyWindow(m_window);
  std::cout << "[GameContext] SDL Window destroyed\n";
  std::cout << "[GameContext] GameContext ended\n";
}