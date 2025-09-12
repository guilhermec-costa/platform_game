#include "../../include/objects/player_object.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

void PlayerObject::update(float dt) {
  if (!on_ground) {
    velocity.y += gravity * dt;
  }

  float max_fall_speed = 800.0f;
  if (velocity.y > max_fall_speed) {
    velocity.y = max_fall_speed;
  }

  position.x += velocity.x * dt;
  position.y += velocity.y * dt;

  if (position.y >= base_height) {
    position.y = base_height;
    velocity.y = 0;
    set_on_ground(true);
  }

  this->collider_comp.set_position({position.x, position.y });
  if (on_ground && velocity.x != 0) {
      animated_sprite.play_animation("run");
  } else if (on_ground) {
      animated_sprite.play_animation("idle");
  } else {
      animated_sprite.play_animation("jump");
  }

  animated_sprite.update(dt);
}

void PlayerObject::render(SDL_Renderer* renderer, const Camera& camera) {
  animated_sprite.render(renderer, position, camera);
}

void PlayerObject::handle_event(PlayerEvent event) {
  switch (event) {
    case JUMP:
      if (on_ground) {
        velocity.y = -jump_force;
        on_ground  = false;
      }
      break;

    case MOVE_LEFT:
      velocity.x = -move_speed;
      animated_sprite.set_flipped(true);
      break;

    case MOVE_RIGHT:
      velocity.x = move_speed;
      animated_sprite.set_flipped(false);
      break;

    case STOP_HORIZONTAL:
      velocity.x = 0;
      break;

    case LAND:
      velocity.y = 0;
      on_ground  = true;
      break;
  }
}
