#include "../../include/game_state.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>

PlayState::PlayState(GameContext& ctx) : GameState(ctx) {
  bg_parallax    = BackgroundParallax(ctx.renderer);
  float ground_y = SDLBackend::get_window_dimension(ctx.window).y - 64;
  ground = Ground(ctx.renderer, ground_y, (int)SDLBackend::get_window_dimension(ctx.window).x);
  player = new PlayerObject(Vector2D(100, 100), Vector2D(80, 80));
  entity_manager.add_entity(player);
}

void PlayState::update(float dt) {
    entity_manager.updateAll(dt);

    float player_collision_offset = 30.0f;
    SDL_Rect player_rect = player->collider_comp.get_rect();
    for (auto& tile : ground.get_tiles()) {
        auto& tile_collider = tile.get_collider();

        if (player->velocity.y >= 0 && tile_collider.check_collision(player_rect)) {
            player->position.y =
                tile_collider.get_rect().y - player->dimension.y + player_collision_offset;
            player->velocity.y = 0;
            player->set_on_ground(true);
            break;
        }
    }

    // Controle da câmera
    float left_margin  = 200;
    float right_margin = 400;

    if (player->position.x - context.camera.x < left_margin) {
        context.camera.x = player->position.x - left_margin;
    } else if (player->position.x - context.camera.x > right_margin) {
        context.camera.x = player->position.x - right_margin;
    }

    if (player->position.x < 0) player->position.x = 0;
    if (context.camera.x < 0) context.camera.x = 0;

    bg_parallax.update(context.camera.x);
}


void PlayState::render() {
  bg_parallax.render(context.window, context.renderer);
  ground.render(context.renderer, context.camera);
  entity_manager.renderAll(context.renderer, context.camera);
  player->collider_comp.render_collision_box(context.renderer, context.camera, true);
  for (auto& tile : ground.get_tiles()) {
    auto& tile_collider = tile.get_collider();
    tile_collider.render_collision_box(context.renderer, context.camera, false);
  }
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
