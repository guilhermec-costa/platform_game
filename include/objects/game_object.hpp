#pragma once

#include "../camera.hpp"
#include "../shared.hpp"

#include <SDL2/SDL_render.h>

class GameObject {
public:
  GameObject() : position(0, 0), velocity(0, 0), direction(0) {}
  GameObject(const GameObject&) = delete;
  GameObject(Vector2D pos, Vector2D vel, Vector2D dimension)
      : position(pos), velocity(vel), dimension(dimension), direction(0) {}
  virtual void update(float dt)                                     = 0;
  virtual void render(SDL_Renderer* renderer, const Camera& camera) = 0;

public:
  const Vector2D& get_position() const { return position; }
  const Vector2D& get_velocity() const { return velocity; }
  const Vector2D& get_dimension() const { return dimension; }

protected:
  Vector2D position;
  Vector2D velocity;
  Vector2D dimension;
  int      direction;
};
