#pragma once

#include "../components/animated_sprite_component.hpp"
#include "game_object.hpp"

enum class MovementState { IDLE, RUNNING, JUMPING, FALLING, WALKING };

enum class ActionState { NONE, ATTACKING };

enum class CharacterEvent { JUMP, MOVE_LEFT, MOVE_RIGHT, STOP_HORIZONTAL, ATTACK };

enum class CharacterState {
  IDLEING,
  RUNNING,
  JUMPING,
  ATTACKING,
};

class CharacterObject : public GameObject {
public:
  CharacterObject(const Vector2& pos, const Vector2& dim);
  virtual ~CharacterObject();

  void         update(float dt) override;
  void         apply_gravity(float dt);
  void         move(float dt);
  std::string  to_string() const override;
  void         land_on(float surface_y);
  void         update_collider();
  virtual void update_state()             = 0;
  virtual void update_animation(float dt) = 0;
  void         set_on_ground(bool state) {
    on_ground = state;
  }

public:
  bool on_ground       = false;
protected:
  Components::AnimatedSpriteComponent animated_sprite;
  float                               gravity         = 1200.0f;
  float                               move_speed      = 50.0f;
  float                               land_offset_pct = 0.22f;
  float                               max_fall_speed  = 800.0f;
  CharacterState                      state           = CharacterState::IDLEING;
  MovementState                       movement_state  = MovementState::IDLE;
  ActionState                         action_state    = ActionState::NONE;
};
