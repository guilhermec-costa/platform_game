#include "../../include/objects/player_object.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

void PlayerObject::update(float dt) {
  apply_gravity(dt);
  move(dt);
  check_window_collision();
  check_ground_collision();
  update_collider();
  update_animation(dt);
}

void PlayerObject::render(SDL_Renderer* renderer, const Camera& camera) {
  animated_sprite.render(renderer, position, camera);
  // collider_comp.render_collision_box(renderer, camera);
}

void PlayerObject::handle_event(PlayerEvent event) {
  switch (event) {
    case JUMP:
      if (on_ground) {
        velocity.y = -JUMP_FORCE;
        set_on_ground(false);
      }
      break;

    case MOVE_LEFT:
      velocity.x = -MOVE_SPEED;
      animated_sprite.set_flipped(true);
      break;

    case MOVE_RIGHT:
      velocity.x = MOVE_SPEED;
      animated_sprite.set_flipped(false);
      break;

    case STOP_HORIZONTAL:
      velocity.x = 0;
      break;

    case LAND:
      velocity.y = 0;
      set_on_ground(true);
      break;
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

void PlayerObject::update_collider() {
  collider_comp.set_position(position + collider_offset);
}

void PlayerObject::update_animation(float dt) {
  if (on_ground && velocity.x != 0) {
    animated_sprite.play_animation(PlayerAnimation::Running);
  } else if (on_ground) {
    animated_sprite.play_animation(PlayerAnimation::Idle);
  } else {
    animated_sprite.play_animation(PlayerAnimation::Jumping);
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
