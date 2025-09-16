#include "../../include/objects/player_object.hpp"

#include "../../include/sdl_backend.hpp"
#include "../../include/asset_manager/texture_manager.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

PlayerObject::PlayerObject(Vector2 pos, Vector2 dim)
    : GameObject(pos, {0, 0}, dim), on_ground(true), base_height_location(pos.y) {
  auto texture = TextureManager::get_texture(TextureManager::asset_path("assets/images/nigthborne.png"));
  Vector2 tex_dim = SDLBackend::get_texture_dimensions(texture);

  collider_offset = {dimension.x * 0.25f, dimension.y * 0.25f};
  Vector2 collider_dim{dimension.x * 0.55f, dimension.y * 0.55f};
  collider_comp = ColliderComponent(pos + collider_offset, collider_dim);

  animated_sprite = AnimatedSpriteComponent(
      TextureComponent(texture, {0, 0}, {tex_dim.x, tex_dim.y}), 80, 80, 0.1, dim);

  animated_sprite.add_animation(static_cast<int>(PlayerAnimation::IDLE), "idle", 0, 8, 0.099f);
  animated_sprite.add_animation(static_cast<int>(PlayerAnimation::RUN), "run", 8, 12, 0.1f);
  animated_sprite.add_animation(static_cast<int>(PlayerAnimation::JUMP), "jump", 0, 8, 0.1f);
  animated_sprite.add_animation(static_cast<int>(PlayerAnimation::ATTACK), "attack", 15, 25, 0.03f,
                                false);
};

void PlayerObject::update(float dt) {
  apply_gravity(dt);
  move(dt);
  check_window_collision();
  check_ground_collision();
  update_collider();
  update_state();
  update_animation(dt);
}

void PlayerObject::render(SDL_Renderer* renderer, const Camera& camera) {
  animated_sprite.render(renderer, position, camera);
}

void PlayerObject::handle_event(PlayerEvent event) {
  switch (event) {
    case PlayerEvent::ATTACK: {
      if (action_state != ActionState::ATTACKING) {
        action_state = ActionState::ATTACKING;
        // velocity.x = 0;
      }
      break;
    }

    case PlayerEvent::MOVE_LEFT: {
      velocity.x = -MOVE_SPEED;
      animated_sprite.set_flipped(true);
      if (on_ground) {
        movement_state = MovementState::RUNNING;
      }
      break;
    }

    case PlayerEvent::MOVE_RIGHT: {
      velocity.x = MOVE_SPEED;
      animated_sprite.set_flipped(false);
      if (on_ground)
        movement_state = MovementState::RUNNING;
      break;
    }

    case PlayerEvent::JUMP: {
      if (on_ground) {
        velocity.y = -JUMP_FORCE;
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
    velocity.y += GRAVITY * dt;
    if (velocity.y > MAX_FALL_SPEED) {
      velocity.y = MAX_FALL_SPEED;
    }
  }
}

void PlayerObject::move(float dt) {
  position += velocity * dt;
}

void PlayerObject::check_ground_collision() {
  if (position.y >= base_height_location) {
    position.y = base_height_location;
    velocity.y = 0;
    set_on_ground(true);
  }
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
  collider_comp.set_position(position + collider_offset);
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

void PlayerObject::check_window_collision() {
  if (collider_comp.position.x < 0) {
    float delta              = 0 - collider_comp.position.x;
    collider_comp.position.x = 0;
    position.x += delta;
  }
}
