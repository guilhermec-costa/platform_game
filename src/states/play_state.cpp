#include "../../include/game_state.hpp"

PlayState::PlayState(GameContext& ctx) : GameState(ctx) {}

void PlayState::update(float dt) {
  entity_manager.updateAll(dt);
}

void PlayState::render() {
  entity_manager.renderAll();
}

void PlayState::handle_event(SDL_Event& event) {}