#include "../../include/game_state.hpp"
#include "../../include/ui/label_element.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>

PlayState::PlayState() : GameState(), bg_parallax() {
  Vector2 win_dim = ctx.window.get_dimension();

  const float tile_side   = ctx.get_world_data().ground_tile_side;
  float       tile_height = win_dim.y - tile_side;
  ground                  = Ground(ctx.renderer, win_dim.x, win_dim.y, tile_height, tile_side);

  player = std::make_unique<PlayerObject>(ctx.get_player_data());

  auto& platforms_data = ctx.get_platforms_data();
  for (const auto& p : platforms_data) {
    platforms.push_back(std::make_unique<PlatformObject>(p.position, p.dimension));
  }

  auto label = std::make_unique<UI::Label>(ctx.get_level().name, ctx.font, Vector2{100, 100},
                                           Vector2{100, 100});
  ctx.ui_manager.add_element(std::move(label));
}

void PlayState::update(float dt) {
  auto& world_data = ctx.get_world_data();
  ground.update(ctx.camera.get_position().x);
  player->update(dt);
  for (auto& platform : platforms) {
    platform->update(dt);
  }
  check_player_ground_collision();
  check_player_platform_collision();
  check_player_window_collision();
  ctx.camera.follow(player->get_position());
  ctx.camera.update(world_data.min_horizontal_x, world_data.max_horizontal_x);
  bg_parallax.update(ctx.camera.get_position().x);
}

void PlayState::check_player_platform_collision() {
  SDL_Rect player_rect = player->get_collider_component().get_rect();

  for (auto& p : platforms) {
    const SDL_Rect& plt_rect = p->get_collider().get_rect();

    if (SDL_HasIntersection(&player_rect, &plt_rect)) {
      RectOverlap overlap = p->get_overlap(player_rect);

      int min_dx = std::min(overlap.left, overlap.right);
      int min_dy = std::min(overlap.top, overlap.bottom);

      if (min_dx < min_dy) {
        if (overlap.left < overlap.right) {
          player->position.x -= overlap.left;
          if(player->velocity.x > 0) {
            player->velocity.x = 0;
          }
        } else {
          player->position.x += overlap.right;
          if(player->velocity.x < 0) {
            player->velocity.x = 0;
          }
        }
      } else {
        if (overlap.top < overlap.bottom) {
          float plt_top     = static_cast<float>(plt_rect.y);
          auto  player_data = ctx.get_player_data();

          player->land_on(plt_top);
        } else {
          player->position.y += overlap.bottom;
          player->velocity.y = 0;
        }
      }
    }
  }
}

void PlayState::check_player_ground_collision() {
  const SDL_Rect& player_rect = player->get_collider_component().get_rect();
  const SDL_Rect& ground_rect = ground.get_collider_component().get_rect();

  if (SDL_HasIntersection(&player_rect, &ground_rect)) {
    float ground_top = static_cast<float>(ground_rect.y);
    player->land_on(ground_top);
  } else {
    player->set_on_ground(false);
  }
}

void PlayState::check_player_window_collision() {
  auto&       player_collider  = player->get_collider_component();
  const auto& world_data       = ctx.get_world_data();
  float       min_horizontal_x = world_data.min_horizontal_x;
  float       max_horizontal_x = world_data.max_horizontal_x;

  if (player_collider.position.x < min_horizontal_x) {
    float delta = min_horizontal_x - player_collider.position.x;
    player->position.x += delta;
    player_collider.position.x = min_horizontal_x;
  }

  float player_right = player_collider.position.x + player_collider.dimension.x;
  if (player_right > max_horizontal_x) {
    float delta = player_right - max_horizontal_x;
    player->position.x -= delta;
    player_collider.position.x -= delta;
  }
}

void PlayState::render() {
  bg_parallax.render(ctx.renderer);
  ground.render(ctx.renderer, ctx.camera);
  for (auto& platform : platforms) {
    platform->render(ctx.renderer, ctx.camera);
  }
  player->render(ctx.renderer, ctx.camera);
  ctx.ui_manager.render(ctx.renderer);
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
    ctx.camera.resize(window.data1);
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