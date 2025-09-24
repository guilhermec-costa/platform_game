#ifndef PROJECT_SOURCE_DIR
#error "Failed to load project source directory"
#endif

#include <memory>

#include <SDL2/SDL_timer.h>

#include "../include/game.hpp"
#include "../include/logger.hpp"


int main() {
  Core::Logger::init();
  LOG_INFO("Hello world");

  Core::GameSpecification game_spec;
  game_spec.window_spec.title  = "Genius Game platform";
  game_spec.window_spec.height = 600;
  game_spec.window_spec.width  = 900;

  Core::Game game(game_spec);
  game.push_layer(std::make_unique<PlayLayer>());
  game.push_layer(std::make_unique<UILayer>());
  game.set_fps(120);
  game.run();
  return 0;
}
