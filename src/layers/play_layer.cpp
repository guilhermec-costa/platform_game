#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>

#include "../../include/asset_manager/audio_manager.hpp"
#include "../../include/game_layer.hpp"

PlayLayer::PlayLayer() : GameLayer(), bg_parallax() {
  Vector2 win_dim = ctx.window.get_dimension();

  const float tile_side   = ctx.get_world_data().ground_tile_side;
  float       tile_height = win_dim.y - tile_side;

  player = std::make_unique<PlayerObject>(ctx.get_player_data());

  const auto& active_level = ctx.get_active_level();
  for (const auto& p : active_level.platforms) {
    platforms.push_back(std::make_unique<PlatformObject>(p));
  }
  for (const auto& m : active_level.monsters) {
    monsters.push_back(std::make_unique<MonsterObject>(m));
  }

  ctx.audio_manager.play_sound(GameAudioChannel::FOREST_AMBIENCE);
  ctx.global_ground = Ground(win_dim.x, win_dim.y, tile_height, tile_side);
}

void PlayLayer::update(float dt) {
  auto& world_data = ctx.get_world_data();
  ctx.global_ground.update(ctx.camera.get_position().x);
  player->update(dt);
  for (const auto& platform : platforms)
    platform->update(dt);
  for (const auto& monster : monsters)
    monster->update(dt);

  check_player_platform_collision();
  check_monster_platform_collision();
  ctx.camera.follow(player->get_collider_component().position);
  ctx.camera.update(world_data.min_horizontal_x, world_data.max_horizontal_x);
  bg_parallax.update(ctx.camera.get_position().x);
}

void PlayLayer::check_monster_platform_collision() {
  for (auto& m : monsters) {
    SDL_Rect m_rect = m->get_collider_component().get_rect();

    for (const auto& p : platforms) {
      const SDL_Rect& plt_rect = p->get_collider().get_rect();

      if (SDL_HasIntersection(&m_rect, &plt_rect)) {
        RectOverlap overlap = p->get_overlap(m_rect);

        int min_dx = std::min(overlap.left, overlap.right);
        int min_dy = std::min(overlap.top, overlap.bottom);

        if (min_dx < min_dy) {
          if (overlap.left < overlap.right) {
            m->position.x -= overlap.left;
            if (m->velocity.x > 0) {
              m->velocity.x = 0;
            }
          } else {
            m->position.x += overlap.right;
            if (m->velocity.x < 0) {
              m->velocity.x = 0;
            }
          }
        } else {
          if (overlap.top < overlap.bottom) {
            float plt_top = static_cast<float>(plt_rect.y);
            m->land_on(plt_top);
          } else {
            m->position.y += overlap.bottom;
            m->velocity.y = 0;
          }
        }
      }
    }
  }
}


void PlayLayer::check_player_platform_collision() {
  SDL_Rect player_rect = player->get_collider_component().get_rect();

  for (const auto& p : platforms) {
    const SDL_Rect& plt_rect = p->get_collider().get_rect();

    if (SDL_HasIntersection(&player_rect, &plt_rect)) {
      RectOverlap overlap = p->get_overlap(player_rect);

      int min_dx = std::min(overlap.left, overlap.right);
      int min_dy = std::min(overlap.top, overlap.bottom);

      if (min_dx < min_dy) {
        if (overlap.left < overlap.right) {
          player->position.x -= overlap.left;
          if (player->velocity.x > 0) {
            player->velocity.x = 0;
            player->velocity.y = 0;
          }
        } else {
          player->position.x += overlap.right;
          if (player->velocity.x < 0) {
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

void PlayLayer::render() {
  bg_parallax.render();
  ctx.global_ground.render();
  for (const auto& platform : platforms) {
    platform->render(ctx.renderer, ctx.camera);
  }
  for (const auto& monster : monsters)
    monster->render(ctx.renderer, ctx.camera);
  player->render(ctx.renderer, ctx.camera);
  ctx.camera.render(ctx.renderer);
}

void PlayLayer::handle_event(const SDL_Event& event) {
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

void PlayLayer::handle_mouse_click_event(const SDL_MouseButtonEvent& button) {
  if (button.type == SDL_MOUSEBUTTONDOWN && button.button == SDL_BUTTON_LEFT) {
    player->handle_event(PlayerEvent::ATTACK);
  }
}

void PlayLayer::handle_window_event(const SDL_WindowEvent& window) {
  if (window.event == SDL_WINDOWEVENT_RESIZED) {
    ctx.global_ground.resize(window.data1, window.data2);
    ctx.camera.resize(window.data1);
    for (const auto& platform : platforms) {
      platform->resize();
    }
    player->resize();
  }
}

void PlayLayer::handle_keydown(const SDL_KeyboardEvent& key) {
  if (key.repeat != 0)
    return;

  switch (key.keysym.sym) {
    case SDLK_SPACE:
      player->handle_event(PlayerEvent::JUMP);
      break;
    case SDLK_d: {
      player->handle_event(PlayerEvent::MOVE_RIGHT);
      break;
    }
    case SDLK_a:
      player->handle_event(PlayerEvent::MOVE_LEFT);
      break;
  }
}

void PlayLayer::handle_keyup(const SDL_KeyboardEvent& key) {
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

bool PlayLayer::is_key_down(SDL_Scancode scancode) const {
  const Uint8* state = SDL_GetKeyboardState(nullptr);
  return state[scancode];
}