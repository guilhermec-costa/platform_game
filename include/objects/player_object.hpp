#pragma once

#include "game_object.hpp"

enum PlayerEvent { JUMP, MOVE_LEFT, MOVE_RIGHT, STOP_HORIZONTAL, LAND };

class PlayerObject : public GameObject {
public:
  PlayerObject() = delete;
  PlayerObject(Vector2D pos, Vector2D _dim)
      : GameObject(pos, {0, 0}), on_ground(true), move_speed(200.0f), jump_force(700.0f),
        gravity(1200.0f) {
    dimension = _dim;
  }

  void handle_event(PlayerEvent event);
  void update(float dt) override;
  void render(SDL_Renderer* renderer, const Camera& camera) override;

private:
  bool  on_ground;
  float move_speed;
  float jump_force;
  float gravity;
};