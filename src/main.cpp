#ifndef PROJECT_SOURCE_DIR
#error "Failed to load project source directory"
#endif

#include "../include/game.hpp"
#include "../include/game_context.hpp"

#include <SDL2/SDL_timer.h>
#include <memory>

int main() {
  Core::GameSpecification game_spec;
  game_spec.window_spec.title  = "Genius Game platform";
  game_spec.window_spec.height = 600;
  game_spec.window_spec.width  = 900;

  Core::Game game(game_spec);
  game.change_state(std::make_unique<PlayState>());
  game.set_fps(120);
  game.run();
  return 0;
}
