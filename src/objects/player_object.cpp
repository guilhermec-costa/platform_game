#include "../../include/objects/player_object.hpp"

#include "../../include/asset_manager/texture_manager.hpp"
#include "../../include/asset_manager/utils.hpp"
#include "../../include/sdl_backend.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

PlayerObject::PlayerObject(LevelMetadata::Player& player_data)
    : GameObject(player_data.start_position, {0, 0}, player_data.dimension), on_ground(true) {
  auto    texture = TextureManager::get_texture(asset_path("assets/images/nigthborne.png"));
  Vector2 tex_dim = SDLBackend::get_texture_dimensions(texture);

  collision_offset_pct = player_data.collision_offset_pct;
  collider_offset = {dimension.x * 0.35f, dimension.y * 0.4f};
  Vector2 collider_dim{dimension.x * 0.40f, dimension.y * 0.40f};
  collider_component = Components::ColliderComponent(player_data.start_position + collider_offset, collider_dim);

  animated_sprite = Components::AnimatedSpriteComponent(
      Components::TextureComponent(texture, {0, 0}, {tex_dim.x, tex_dim.y}), 80, 80, 0.1,  player_data.dimension);

  animated_sprite.add_animation(static_cast<int>(PlayerAnimation::IDLE), "idle", 0, 8, 0.099f);
  animated_sprite.add_animation(static_cast<int>(PlayerAnimation::RUN), "run", 8, 12, 0.1f);
  animated_sprite.add_animation(static_cast<int>(PlayerAnimation::JUMP), "jump", 0, 8, 0.1f);
  animated_sprite.add_animation(static_cast<int>(PlayerAnimation::ATTACK), "attack", 15, 25, 0.03f,
                                false);

  move_spped     = player_data.attrs.move_speed;
  gravity        = player_data.attrs.gravity;
  jump_force     = player_data.attrs.jump_force;
  max_fall_speed = player_data.attrs.max_fall_speed;
};

void PlayerObject::update(float dt) {
  apply_gravity(dt);
  move(dt);
  update_collider();
  update_state();
  update_animation(dt);
}

void PlayerObject::render(SDL_Renderer* renderer, const Camera& camera) {
  animated_sprite.render(renderer, position, camera);
  collider_component.render_collision_box(renderer, camera);
}

void PlayerObject::handle_event(PlayerEvent event) {
  switch (event) {
    case PlayerEvent::ATTACK: {
      if (action_state != ActionState::ATTACKING) {
        action_state = ActionState::ATTACKING;
      }
      break;
    }

    case PlayerEvent::MOVE_LEFT: {
      velocity.x = -move_spped;
      animated_sprite.set_flipped(true);
      if (on_ground) {
        movement_state = MovementState::RUNNING;
      }
      break;
    }

    case PlayerEvent::MOVE_RIGHT: {
      velocity.x = move_spped;
      animated_sprite.set_flipped(false);
      if (on_ground)
        movement_state = MovementState::RUNNING;
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
      if (on_ground)
        movement_state = MovementState::IDLE;
      break;
    }
  }
}

void PlayerObject::apply_gravity(float dt) {
  if (!on_ground) {
    velocity.y += gravity * dt;
    if (velocity.y > max_fall_speed) {
      velocity.y = max_fall_speed;
    }
  }
}

void PlayerObject::move(float dt) {
  position += velocity * dt;
}

void PlayerObject::update_state() {
  if (!on_ground) {
    movement_state = MovementState::JUMPING;
  } else if (velocity.x != 0) {
    movement_state = MovementState::RUNNING;
  } else {
    movement_state = MovementState::IDLE;
  }

  if (action_state == ActionState::ATTACKING && animated_sprite.is_finished()) {
    action_state = ActionState::NONE;
  }
}

void PlayerObject::update_collider() {
  collider_component.set_position(position + collider_offset);
}

void PlayerObject::update_animation(float dt) {
  if (action_state == ActionState::ATTACKING) {
    animated_sprite.play_animation((int)PlayerAnimation::ATTACK);
  } else {
    switch (movement_state) {
      case MovementState::JUMPING:
        animated_sprite.play_animation((int)PlayerAnimation::JUMP);
        break;
      case MovementState::RUNNING:
        animated_sprite.play_animation((int)PlayerAnimation::RUN);
        break;
      case MovementState::IDLE:
        animated_sprite.play_animation((int)PlayerAnimation::IDLE);
        break;
      default:
        break;
    }
  }
  animated_sprite.update(dt);
}

void PlayerObject::land_on(float surface_y) {
  float new_y = surface_y - dimension.y  +
                         (dimension.y * collision_offset_pct);
    position.y = new_y; 
    velocity.y = 0;
    on_ground  = true;
}