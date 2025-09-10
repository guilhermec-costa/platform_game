#include "../../include/game_state.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>

PlayState::PlayState(GameContext& ctx) : GameState(ctx) {
  bg_parallax      = BackgroundParallax(ctx.renderer);
  Vector2D win_dim = SDLBackend::get_window_dimension(ctx.window);

  const int tile_width    = 64;
  float     ground_y      = win_dim.y - tile_width;
  ground                  = Ground(ctx.renderer, ctx.window, ground_y, tile_width);
  const int player_height = 200;
  TextureManager::get_instance().load_texture("assets/nigthborne.png", ctx.renderer);
  player = new PlayerObject(Vector2D(100, ground_y - player_height + ground.collision_offset),
                            Vector2D(player_height, player_height));
}

void PlayState::update(float dt) {
  player->update(dt);
  SDL_Rect player_rect     = player->collider_comp.get_rect();
  auto&    ground_collider = ground.get_collider_component();
  if (!player->on_ground && ground_collider.check_collision(player_rect)) {
    player->position.y = ground_collider.get_rect().y - player->dimension.y;
    player->velocity.y = 0;
    player->set_on_ground(true);
  }

  float left_margin  = 100;
  float right_margin = 650;

  if (player->position.x - context.camera.x < left_margin) {
    context.camera.x = player->position.x - left_margin;
  } else if (player->position.x - context.camera.x > right_margin) {
    context.camera.x = player->position.x - right_margin;
  }

  if (player->position.x < 0)
    player->position.x = 0;
  if (context.camera.x < 0)
    context.camera.x = 0;
  bg_parallax.update(context.camera.x);
}

void PlayState::render() {
  bg_parallax.render(context.window, context.renderer);
  ground.render(context.renderer, context.camera);
  player->render(context.renderer, context.camera);
  player->collider_comp.render_collision_box(context.renderer, context.camera);
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
