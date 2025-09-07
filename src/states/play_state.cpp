#include "../../include/game_state.hpp"

PlayState::PlayState(GameContext& ctx) : GameState(ctx) {
  bg_parallax = BackgroundParallax(ctx.renderer);
}

void PlayState::update(float dt) {
  bg_parallax.update(dt);
  entity_manager.updateAll(dt);
}

void PlayState::render() {
  bg_parallax.render(context.window, context.renderer);
}

void PlayState::handle_event(SDL_Event& event) {}