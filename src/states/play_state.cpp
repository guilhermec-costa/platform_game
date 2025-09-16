#include "../../include/game_state.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>

PlayState::PlayState(GameContext& ctx) : GameState(ctx) {
  bg_parallax     = BackgroundParallax();
  Vector2 win_dim = SDLBackend::get_window_dimension(ctx.window);

  const int tile_width = 86;
  float     ground_y   = win_dim.y - tile_width;
  ground               = Ground(ctx.renderer, win_dim.x, win_dim.y, ground_y, tile_width);

  const int   player_height        = 320;
  const float base_height_location = ground_y - player_height + 100;
  player   = std::make_unique<PlayerObject>(Vector2(ctx.active_level.player_start.x, base_height_location),
                                            Vector2(player_height, player_height));
  platform = std::make_unique<PlatformObject>(Vector2(100, base_height_location),
                                              Vector2(player_height, player_height));
}

void PlayState::update(float dt) {
  player->update(dt);
  player->check_ground_collision();
  context.camera.follow(player->get_position());
  context.camera.update();
  bg_parallax.update(context.camera.get_position().x);
}

void PlayState::render() {
  bg_parallax.render(context.window, context.renderer);
  ground.render(context.renderer, context.camera);
  player->render(context.renderer, context.camera);
  platform->render(context.renderer, context.camera);
}

void PlayState::handle_event(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      handle_mouse_click_event(event.button);
      break;
    }
    case SDL_WINDOWEVENT: {
      handle_window_event(event.window);
      break;
    }
    case SDL_KEYDOWN: {
      handle_keydown(event.key);
      break;
    }
    case SDL_KEYUP: {
      handle_keyup(event.key);
      break;
    }
  }
}

void PlayState::handle_mouse_click_event(const SDL_MouseButtonEvent& button) {
  if (button.type == SDL_MOUSEBUTTONDOWN && button.button == SDL_BUTTON_LEFT) {
    player->handle_event(PlayerEvent::ATTACK);
  }
}

void PlayState::handle_window_event(const SDL_WindowEvent& window) {
  if (window.event == SDL_WINDOWEVENT_RESIZED) {
    ground.resize(window.data1, window.data2);
    context.camera.resize(window.data1);
  }
}

void PlayState::handle_keydown(const SDL_KeyboardEvent& key) {
  if (key.repeat != 0)
    return;

  switch (key.keysym.sym) {
    case SDLK_SPACE:
      player->handle_event(PlayerEvent::JUMP);
      break;
    case SDLK_d:
      player->handle_event(PlayerEvent::MOVE_RIGHT);
      break;
    case SDLK_a:
      player->handle_event(PlayerEvent::MOVE_LEFT);
      break;
  }
}

void PlayState::handle_keyup(const SDL_KeyboardEvent& key) {
  switch (key.keysym.sym) {
    case SDLK_d: {
      if (!is_key_down(SDL_SCANCODE_A)) {
        player->handle_event(PlayerEvent::STOP_HORIZONTAL);
      }
      break;
    }
    case SDLK_a:
      if (!is_key_down(SDL_SCANCODE_D)) {
        player->handle_event(PlayerEvent::STOP_HORIZONTAL);
      }
      break;
  }
}

bool PlayState::is_key_down(SDL_Scancode scancode) const {
  const Uint8* state = SDL_GetKeyboardState(nullptr);
  return state[scancode];
}