#pragma once

#include "../../include/level.hpp"
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
  PlayerObject(LevelMetadata::Player& player_data);

  void        handle_event(PlayerEvent event);
  void        update(float dt) override;
  void        render(SDL_Renderer* renderer, const Core::Camera& camera) override;
  void        apply_gravity(float dt);
  void        move(float dt);
  void        update_collider();
  void        update_animation(float dt);
  void        update_state();
  void        land_on(float surface_y);
  inline void set_on_ground(bool state) { on_ground = state; }
  inline Components::ColliderComponent& get_collider_component() { return collider_component; }

public:
  bool  on_ground;
  float move_spped;
  float jump_force;
  float gravity;
  float max_fall_speed;

private:
  Components::ColliderComponent       collider_component;
  Components::AnimatedSpriteComponent animated_sprite;
  Vector2                             collider_offset;
  PlayerState                         state;
  MovementState                       movement_state = MovementState::IDLE;
  ActionState                         action_state   = ActionState::NONE;
  float                               collision_offset_pct;
};