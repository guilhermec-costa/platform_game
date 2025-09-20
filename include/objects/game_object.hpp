#pragma once

#include "../camera.hpp"
#include "../shared.hpp"

#include <SDL2/SDL_render.h>

class GameObject {
public:
  GameObject() : position(0, 0), velocity(0, 0), direction(0) {}
  GameObject(const GameObject&) = delete;
  GameObject(Vector2 pos, Vector2 vel, Vector2 dimension)
      : position(pos), velocity(vel), dimension(dimension), direction(0) {}
  virtual void update(float dt)                                           = 0;
  virtual void render(SDL_Renderer* renderer, const Core::Camera& camera) = 0;

public:
  Vector2 position;
  Vector2 velocity;
  Vector2 dimension;
  int     direction;
};
