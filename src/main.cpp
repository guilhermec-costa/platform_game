#ifndef PROJECT_SOURCE_DIR
#error "Failed to load project source directory"
#endif

#include "../include/game.hpp"
#include "../include/game_context.hpp"
#include "../include/sdl_backend.hpp"

#include <SDL2/SDL_timer.h>
#include <memory>

int main() {
  SDLBackend::init_subsytems();
  auto window   = SDLBackend::create_window(900, 600);
  auto renderer = SDLBackend::create_renderer(window);
  GameContext::instance().init(window, renderer);

  Game game;
  game.change_state(std::make_unique<PlayState>());
  game.set_fps(60);
  game.run();
  return 0;
}
