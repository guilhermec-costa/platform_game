#include "../include/game.hpp"

Game::Game(GameContext& ctx) : running(true), current_state(nullptr), context(ctx) {
  fps_counter = FPSCounter();
}

void Game::handle_events() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      quit();
    }
    if (current_state) {
      current_state->handle_event(event);
    }
  }
}

void Game::update() {
  // float deltaTime = /* calcula o tempo entre frames */;
  if (current_state) {
    current_state->update(0);
  }
}

void Game::render() {
  SDL_SetRenderDrawColor(context.renderer(), 0, 0, 0, 255);
  SDL_RenderClear(context.renderer());

  if (current_state) {
    current_state->render();
  }

  SDL_RenderPresent(context.renderer());
}
