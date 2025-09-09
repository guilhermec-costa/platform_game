#pragma once

#include "../components/collider_component.hpp"
#include "game_object.hpp"

enum PlayerEvent { JUMP, MOVE_LEFT, MOVE_RIGHT, STOP_HORIZONTAL, LAND };

class PlayerObject : public GameObject {
public:
  PlayerObject() = delete;
  PlayerObject(Vector2D pos, Vector2D dim)
      : GameObject(pos, {0, 0}, dim), on_ground(true), move_speed(300.0f), jump_force(600.0f),
        base_height(pos.y), gravity(1200.0f) {
    collider_comp = ColliderComponent(pos, dim);
  }

  void        handle_event(PlayerEvent event);
  void        update(float dt) override;
  void        render(SDL_Renderer* renderer, const Camera& camera) override;
  inline void set_on_ground(bool state) { on_ground = state; }

public:
  ColliderComponent collider_comp;
  float             base_height;

private:
  bool  on_ground;
  float move_speed;
  float jump_force;
  float gravity;
};