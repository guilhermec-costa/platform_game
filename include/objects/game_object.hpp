#pragma once

#include "../camera.hpp"
#include "../shared.hpp"

#include <SDL2/SDL_render.h>

class GameObject {
public:
  GameObject() : position(0, 0), velocity(0, 0), direction(0) {}
  GameObject(const GameObject&) = delete;
  GameObject(Vector2D pos, Vector2D vel) : position(pos), velocity(vel), direction(0) {}
  virtual void update(float dt)                                     = 0;
  virtual void render(SDL_Renderer* renderer, const Camera& camera) = 0;

public:
  Vector2D position;
  Vector2D velocity;
  Vector2D dimension;
  int      direction;
};

enum PlayerEvent { JUMP, MOVE_LEFT, MOVE_RIGHT, STOP_HORIZONTAL, LAND };

class PlayerObject : public GameObject {
public:
  PlayerObject() = delete;
  PlayerObject(Vector2D pos, Vector2D _dim)
      : GameObject(pos, {0, 0}), on_ground(true), move_speed(400.0f), jump_force(900.0f),
        gravity(2500.0f) {
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