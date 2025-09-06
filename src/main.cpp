#include "../include/game.hpp"
#include "../include/game_context.hpp"
#include "../include/sdl_backend.hpp"

#include <SDL2/SDL_timer.h>

int main() {
  if (SDLBackend::init_subsytems() == -1) {
    return -1;
  }
  auto        window   = SDLBackend::create_window(900, 600);
  auto        renderer = SDLBackend::create_renderer(window);
  GameContext ctx(window, renderer);

  Game game(ctx);
  game.set_fps(120);
  game.run();
  return 0;
}
