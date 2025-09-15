#pragma once

#include "../components/animated_sprite_component.hpp"
#include "../components/collider_component.hpp"
#include "game_object.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

enum class MovementState { IDLE, RUNNING, JUMPING, FALLING };
enum class ActionState { NONE, ATTACKING };

enum class PlayerEvent { JUMP, MOVE_LEFT, MOVE_RIGHT, STOP_HORIZONTAL, ATTACK };
enum class PlayerAnimation { RUN, JUMP, IDLE, ATTACK };
enum class PlayerState {
  IDLEING,
  RUNNING,
  JUMPING,
  ATTACKING,
};

class PlayerObject : public GameObject {
public:
  PlayerObject() = delete;
  PlayerObject(Vector2D pos, Vector2D dim);

  void        handle_event(PlayerEvent event);
  void        update(float dt) override;
  void        render(SDL_Renderer* renderer, const Camera& camera) override;
  void        apply_gravity(float dt);
  void        move(float dt);
  void        check_ground_collision();
  void        update_collider();
  void        update_animation(float dt);
  void        update_state();
  void        check_window_collision();
  inline void set_on_ground(bool state) { on_ground = state; }

public:
  float base_height_location;
  bool  on_ground;

public:
  static constexpr float MOVE_SPEED     = 400.0f;
  static constexpr float JUMP_FORCE     = 600.0f;
  static constexpr float GRAVITY        = 1400.0f;
  static constexpr float MAX_FALL_SPEED = 800.0f;

private:
  ColliderComponent       collider_comp;
  AnimatedSpriteComponent animated_sprite;
  Vector2D                collider_offset;
  PlayerState             state;
  MovementState           movement_state = MovementState::IDLE;
  ActionState             action_state   = ActionState::NONE;
};