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
#include "../../include/asset_manager/json_manager.hpp"
#include "../../include/game_layer.hpp"

PlayLayer::PlayLayer() : GameLayer(), bg_parallax() {
  Vector2 win_dim = ctx.window.get_dimension();

  const float tile_side   = ctx.get_world_data().ground_tile_side;
  float       tile_height = win_dim.y - tile_side;

  ctx.global_ground = Ground(win_dim.x, win_dim.y, tile_height, tile_side);

  ctx.audio_manager.play_sound(GameAudioChannel::FOREST_AMBIENCE);

  update_level();
}

void PlayLayer::update(float dt) {
  const auto& world_data = ctx.get_world_data();
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

void PlayLayer::resolve_platform_collision(CharacterObject& obj, const PlatformObject& platform) {
  SDL_Rect        obj_rect = obj.get_collider_component().get_rect();
  const SDL_Rect& plt_rect = platform.get_collider_component().get_rect();

  if (!SDL_HasIntersection(&obj_rect, &plt_rect))
    return;

  RectOverlap overlap = platform.get_overlap(obj_rect);

  int min_dx = std::min(overlap.left, overlap.right);
  int min_dy = std::min(overlap.top, overlap.bottom);

  if (min_dx < min_dy) {
    if (overlap.left < overlap.right) {
      obj.position.x -= overlap.left;
      if (obj.velocity.x > 0)
        obj.velocity.x = 0;
    } else {
      obj.position.x += overlap.right;
      if (obj.velocity.x < 0)
        obj.velocity.x = 0;
    }
  } else {
    if (overlap.top < overlap.bottom) {
      float plt_top = static_cast<float>(plt_rect.y);
      obj.land_on(plt_top);
    } else {
      obj.position.y += overlap.bottom;
      obj.velocity.y = 0;
    }
  }
}

void PlayLayer::check_player_platform_collision() {
  for (const auto& p : platforms) {
    resolve_platform_collision(*player, *p);
  }
}

void PlayLayer::check_monster_platform_collision() {
  for (auto& m : monsters) {
    for (const auto& p : platforms) {
      resolve_platform_collision(*m, *p);
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

void PlayLayer::change_level(int level_id) {
  auto it = level_asset_mapping.find(level_id);
  if (it == level_asset_mapping.end())
    return;

  auto*     level_json     = Managers::JSONManager::instance().get_or_load(it->second);
  LevelData new_level_data = LevelData::from_json(
      *level_json, ctx.window.get_height(), ctx.game_data.world_data.max_horizontal_x);
  ctx.set_active_level(new_level_data);

  update_level();
}
void PlayLayer::handle_keydown(const SDL_KeyboardEvent& key) {

  if (key.repeat != 0)
    return;

  switch (key.keysym.sym) {
    case SDLK_SPACE: {
      player->handle_event(PlayerEvent::JUMP);
      break;
    }
    case SDLK_d: {
      player->handle_event(PlayerEvent::MOVE_RIGHT);
      break;
    }
    case SDLK_a: {
      player->handle_event(PlayerEvent::MOVE_LEFT);
      break;
    }
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

void PlayLayer::update_level() {
  platforms.clear();
  monsters.clear();

  const auto& active_level = ctx.get_active_level();

  for (const auto& p : active_level.platforms) {
    platforms.push_back(std::make_unique<PlatformObject>(p));
  }

  for (const auto& m : active_level.monsters) {
    monsters.push_back(std::make_unique<MonsterObject>(m));
  }

  player = std::make_unique<PlayerObject>(ctx.get_player_data());

  Vector2     win_dim     = ctx.window.get_dimension();
  const float tile_side   = ctx.get_world_data().ground_tile_side;
  float       tile_height = win_dim.y - tile_side;

  ctx.global_ground = Ground(win_dim.x, win_dim.y, tile_height, tile_side);

  std::cout << "Level atualizado!\n";
}

bool PlayLayer::is_key_down(SDL_Scancode scancode) const {
  const Uint8* state = SDL_GetKeyboardState(nullptr);
  return state[scancode];
}