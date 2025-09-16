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

  const float tile_side   = ctx.get_world_data().ground_tile_side;
  float       tile_height = win_dim.y - tile_side;
  ground                  = Ground(ctx.renderer, win_dim.x, win_dim.y, tile_height, tile_side);

  auto& player_data = ctx.get_player_data();
  player =
      std::make_unique<PlayerObject>(Vector2(player_data.start_position),
                                     Vector2(player_data.height, player_data.height), player_data);
}

void PlayState::update(float dt) {
  auto& world_data = context.get_world_data();
  ground.update(context.camera.get_position().x);
  player->update(dt);
  check_player_ground_collision();
  check_player_window_collision();
  context.camera.follow(player->get_position());
  context.camera.update(world_data.min_horizontal_x, world_data.max_horizontal_x);
  bg_parallax.update(context.camera.get_position().x);
}

void PlayState::check_player_ground_collision() {
  const SDL_Rect& player_rect = player->get_collider_component().get_rect();
  const SDL_Rect& ground_rect = ground.get_collider_component().get_rect();

  if (SDL_HasIntersection(&player_rect, &ground_rect)) {
    float ground_top  = static_cast<float>(ground_rect.y);
    auto  player_data = context.get_player_data();

    float player_new_y = ground_top - player->get_dimension().y +
                         (player_data.height * player_data.collision_offset_pct);
    player->set_position_y(player_new_y);
    player->set_velocity_y(0);
    player->set_on_ground(true);
  } else {
    player->set_on_ground(false);
  }
}

void PlayState::check_player_window_collision() {
  auto&       player_collider  = player->get_collider_component();
  const auto& world_data       = context.get_world_data();
  float       min_horizontal_x = world_data.min_horizontal_x;
  float       max_horizontal_x = world_data.max_horizontal_x;

  if (player_collider.position.x < min_horizontal_x) {
    float delta = min_horizontal_x - player_collider.position.x;
    player->set_position_x(player->get_position().x + delta);
    player_collider.position.x = min_horizontal_x;
  }

  float player_right = player_collider.position.x + player_collider.dimension.x;
  if (player_right > max_horizontal_x) {
    float delta = player_right - max_horizontal_x;
    player->set_position_x(player->get_position().x - delta);
    player_collider.position.x -= delta;
  }
}

void PlayState::render() {
  bg_parallax.render(context.window, context.renderer);
  ground.render(context.renderer, context.camera);
  player->render(context.renderer, context.camera);
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