#include "../../include/game_state.hpp"
#include "../../include/objects/game_object.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>

PlayState::PlayState(GameContext& ctx) : GameState(ctx) {
  bg_parallax = BackgroundParallax(ctx.renderer);
  player      = new PlayerObject(Vector2D(300, 200), Vector2D(100, 100));
  entity_manager.add_entity(player);
}

void PlayState::update(float dt) {
  entity_manager.updateAll(dt);

  float left_margin  = 200;
  float right_margin = 400;

  if (player->position.x - context.camera.x < left_margin) {
    context.camera.x = player->position.x - left_margin;
  } else if (player->position.x - context.camera.x > right_margin) {
    context.camera.x = player->position.x - right_margin;
  }
  if (player->position.x < 0) {
    player->position.x = 0;
}
  if (context.camera.x < 0) {
    context.camera.x = 0;
  }

  bg_parallax.update(context.camera.x);
}

void PlayState::render() {
  bg_parallax.render(context.window, context.renderer);
  entity_manager.renderAll(context.renderer, context.camera);
}

void PlayState::handle_event(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      if (event.key.repeat == 0) {
        if (event.key.keysym.sym == SDLK_SPACE) {
          player->handle_event(PlayerEvent::JUMP);
        }
        if (event.key.keysym.sym == SDLK_RIGHT) {
          player->handle_event(PlayerEvent::MOVE_RIGHT);
        }
        if (event.key.keysym.sym == SDLK_LEFT) {
          player->handle_event(PlayerEvent::MOVE_LEFT);
        }
      }
      break;
    }

    case SDL_KEYUP: {
      if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_LEFT) {
        player->handle_event(PlayerEvent::STOP_HORIZONTAL);
      }
      break;
    }
  }
}
