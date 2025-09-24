#include "../../include/objects/player_object.hpp"

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "../../include/asset_manager/texture_manager.hpp"
#include "../../include/components/component_factory.hpp"
#include "../../include/game_context.hpp"
#include "../../include/logger.hpp"

PlayerObject::PlayerObject(const PlayerData& data) :
  m_metadata(data), CharacterObject(data.position, data.dimension) {
  auto texture =
      Managers::TextureManagerSingleton::instance().get_asset("assets/images/nigthborne.png");
  Vector2 tex_dim = Managers::TextureManagerSingleton::instance().get_texture_dimension(texture);

  land_offset_pct = data.land_offset_pct;
  Vector2 collider_dim{dimension.x * 0.32f, dimension.y * 0.37f};
  collider_component =
      ComponentFactory::make_collider(data.position,
                                      collider_dim,
                                      Vector2{dimension.x * 0.35f, dimension.y * 0.43f},
                                      "Player Collider");

  sword_collider_component = ComponentFactory::make_collider(collider_component->position,
                                                             collider_component->dimension,
                                                             Vector2{100, 0},
                                                             "Sword Collider");
  sword_collider_component->set_drawing_color({0, 0, 255, SDL_ALPHA_OPAQUE});

  animated_sprite = Components::AnimatedSpriteComponent(
      Components::TextureComponent(texture, {0, 0}, {tex_dim.x, tex_dim.y}),
      80,
      80,
      0.1,
      data.dimension);

  animated_sprite.add_animation("idle", 0, 8, 0.099f);
  animated_sprite.add_animation("run", 8, 12, 0.1f);
  animated_sprite.add_animation("jump", 0, 8, 0.1f);
  animated_sprite.add_animation("attack", 15, 25, 0.03f, false);

  move_speed     = data.attrs.move_speed;
  gravity        = data.attrs.gravity;
  jump_force     = data.attrs.jump_force;
  max_fall_speed = data.attrs.max_fall_speed;
};

void PlayerObject::update(float dt) {
  CharacterObject::update(dt);
  sword_collider_component->set_position(collider_component->position);
  check_player_ground_collision();
  check_player_window_collision();
}

void PlayerObject::check_player_ground_collision() {
  const SDL_Rect& player_rect = collider_component->get_rect();
  const SDL_Rect& ground_rect =
      Core::GameContext::instance().global_ground->get_collider_component().get_rect();

  if (SDL_HasIntersection(&player_rect, &ground_rect)) {
    float ground_top = static_cast<float>(ground_rect.y);
    land_on(ground_top);
  } else {
    set_on_ground(false);
  }
}

void PlayerObject::render(SDL_Renderer* renderer, const Core::Camera& camera) {
  collider_component->render_collision_box(renderer, camera);
  sword_collider_component->render_collision_box(renderer, camera);
  animated_sprite.render(renderer, position, camera);
}

void PlayerObject::handle_event(PlayerEvent event) {
  switch (event) {
    case PlayerEvent::ATTACK: {
      if (action_state != ActionState::ATTACKING) {
        action_state = ActionState::ATTACKING;
        audio_manager.play_sound(GameAudioChannel::SWORD_MISS_ATTACK);
      }
      break;
    }

    case PlayerEvent::MOVE_LEFT: {
      velocity.x = -move_speed;
      animated_sprite.set_flipped(true);
      sword_collider_component->flip_to_left();
      if (on_ground) {
        movement_state = MovementState::RUNNING;
      }
      break;
    }

    case PlayerEvent::MOVE_RIGHT: {
      velocity.x = move_speed;
      animated_sprite.set_flipped(false);
      sword_collider_component->flip_to_right();
      if (on_ground) {
        movement_state = MovementState::RUNNING;
      }
      break;
    }

    case PlayerEvent::JUMP: {
      if (on_ground) {
        velocity.y = -jump_force;
        set_on_ground(false);
        movement_state = MovementState::JUMPING;
      }
      break;
    }

    case PlayerEvent::STOP_HORIZONTAL: {
      velocity.x = 0;
      if (on_ground) {
        movement_state = MovementState::IDLE;
      }
      break;
    }
  }
}

void PlayerObject::update_state() {
  if (!on_ground) {
    movement_state = MovementState::JUMPING;
    audio_manager.stop_channel(GameAudioChannel::WALK_ON_GRASS);
  } else if (velocity.x != 0) {
    movement_state = MovementState::RUNNING;
    if (!audio_manager.channel_playing(GameAudioChannel::WALK_ON_GRASS)) {
      audio_manager.play_sound(GameAudioChannel::WALK_ON_GRASS);
    }
  } else {
    movement_state = MovementState::IDLE;
    audio_manager.stop_channel(GameAudioChannel::WALK_ON_GRASS);
  }

  if (action_state == ActionState::ATTACKING && animated_sprite.is_finished()) {
    action_state = ActionState::NONE;
  }
}

void PlayerObject::update_animation(float dt) {
  if (action_state == ActionState::ATTACKING) {
    animated_sprite.play_animation("attack");
  } else {
    switch (movement_state) {
      case MovementState::JUMPING:
        animated_sprite.play_animation("jump");
        break;
      case MovementState::RUNNING:
        animated_sprite.play_animation("run");
        break;
      case MovementState::IDLE:
        animated_sprite.play_animation("idle");
        break;
      default:
        break;
    }
  }
  animated_sprite.update(dt);
}

void PlayerObject::resize() {
  LOG_INFO("Resizing player");

  auto&       window     = Core::GameContext::instance().window;
  const auto& world_data = Core::GameContext::instance().game_data.world_data;

  // escala apenas horizontal
  float scale_x = static_cast<float>(window.get_width()) / world_data.max_horizontal_x;

  // ajusta posição e escala horizontal
  collider_component->set_scale(Vector2{position.x * scale_x, position.y}); // Y não muda
  animated_sprite.set_scale(Vector2{scale_x, 1.0f});                        // escala só X
}

void PlayerObject::check_player_window_collision() {
  const auto& world_data       = ctx.game_data.world_data;
  float       min_horizontal_x = world_data.min_horizontal_x;
  float       max_horizontal_x = world_data.max_horizontal_x;

  if (collider_component->position.x < min_horizontal_x) {
    float delta = min_horizontal_x - collider_component->position.x;
    position.x += delta;
    collider_component->position.x = min_horizontal_x;
  }

  float player_right = collider_component->position.x + collider_component->dimension.x;
  if (player_right > max_horizontal_x) {
    float delta = player_right - max_horizontal_x;
    position.x -= delta;
    collider_component->position.x -= delta;
  }
}