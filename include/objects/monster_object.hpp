#pragma once

#include <SDL2/SDL_events.h>

#include "../components/animated_sprite_component.hpp"
#include "../components/collider_component.hpp"
#include "../level.hpp"
#include "game_object.hpp"
#include "player_object.hpp"

enum class MonsterAnimation { IDLE, ATTACK };

class MonsterObject : public GameObject {
public:
  MonsterObject() = delete;
  MonsterObject(const LevelData::MonsterData& data);

  void        handle_event(const SDL_Event& event);
  void        update(float dt) override;
  void        render(SDL_Renderer* renderer, const Core::Camera& camera) override;
  void        apply_gravity(float dt);
  void        move(float dt);
  void        update_collider();
  void        update_animation(float dt);
  void        update_state();
  void        land_on(float surface_y);
  void        resize();
  inline void set_on_ground(bool state) {
    on_ground = state;
  }
  

public:
  bool  on_ground;
  float move_spped;
  float jump_force;
  float gravity;
  float max_fall_speed;

private:
  MovementState movement_state = MovementState::IDLE;
  ;
  Components::AnimatedSpriteComponent animated_sprite;
  Vector2                             collider_offset;
};