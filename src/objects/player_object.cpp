#include "../../include/objects/player_object.hpp"

#include "../../include/sdl_backend.hpp"
#include "../../include/texture_manager.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

PlayerObject::PlayerObject(Vector2D pos, Vector2D dim)
    : GameObject(pos, {0, 0}, dim), on_ground(true), base_height_location(pos.y) {
  auto texture = TextureManager::get_texture(TextureManager::asset_path("assets/nigthborne.png"));
  Vector2D tex_dim = SDLBackend::get_texture_dimensions(texture);

  collider_offset = {dimension.x * 0.25f, dimension.y * 0.25f};
  Vector2D collider_dim{dimension.x * 0.55f, dimension.y * 0.55f};
  collider_comp = ColliderComponent(pos + collider_offset, collider_dim);

  animated_sprite = AnimatedSpriteComponent(
      TextureComponent(texture, {0, 0}, {tex_dim.x, tex_dim.y}), 80, 80, 0.1, dim);

  animated_sprite.add_animation(static_cast<int>(PlayerAnimation::IDLE), "idle", 0, 8, 0.099f);
  animated_sprite.add_animation(static_cast<int>(PlayerAnimation::RUN), "run", 8, 12, 0.1f);
  animated_sprite.add_animation(static_cast<int>(PlayerAnimation::JUMP), "jump", 0, 8, 0.1f);
  animated_sprite.add_animation(static_cast<int>(PlayerAnimation::ATTACK), "attack", 15, 25, 0.015f,
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
      state = PlayerState::ATTACKING;
      break;
    }
    case PlayerEvent::MOVE_LEFT: {
      velocity.x = -MOVE_SPEED;
      animated_sprite.set_flipped(true);
      if (on_ground)
        state = PlayerState::RUNNING;
      break;
    }

    case PlayerEvent::MOVE_RIGHT: {
      velocity.x = MOVE_SPEED;
      animated_sprite.set_flipped(false);
      if (on_ground)
        state = PlayerState::RUNNING;
      break;
    }

    case PlayerEvent::JUMP: {
      if (on_ground) {
        velocity.y = -JUMP_FORCE;
        set_on_ground(false);
        state = PlayerState::JUMPING;
      }
      break;
    }

    case PlayerEvent::STOP_HORIZONTAL: {
      velocity.x = 0;
      if (on_ground)
        state = PlayerState::IDLEING;
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
  if (state == PlayerState::ATTACKING) {
    if (animated_sprite.is_finished()) {
      state = on_ground ? (velocity.x != 0 ? PlayerState::RUNNING : PlayerState::IDLEING)
                        : PlayerState::JUMPING;
    }
    return;
  }

  if (!on_ground) {
    state = PlayerState::JUMPING;
  } else if (velocity.x != 0) {
    state = PlayerState::RUNNING;
  } else {
    state = PlayerState::IDLEING;
  }
}

void PlayerObject::update_collider() {
  collider_comp.set_position(position + collider_offset);
}

void PlayerObject::update_animation(float dt) {
  switch (state) {
    case PlayerState::ATTACKING: {
      animated_sprite.play_animation(static_cast<int>(PlayerAnimation::ATTACK));
      break;
    }
    case PlayerState::JUMPING: {
      animated_sprite.play_animation(static_cast<int>(PlayerAnimation::JUMP));
      break;
    }
    case PlayerState::RUNNING: {
      animated_sprite.play_animation(static_cast<int>(PlayerAnimation::RUN));
      break;
    }
    case PlayerState::IDLEING: {
      animated_sprite.play_animation(static_cast<int>(PlayerAnimation::IDLE));
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
