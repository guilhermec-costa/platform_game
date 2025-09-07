#pragma once

#include "../shared.hpp"

class GameObject {
private:
  const char* m_id;

public:
  GameObject()                  = delete;
  GameObject(const GameObject&) = delete;
  GameObject(const char* id) : m_id(id) {};
  GameObject(Vector2D pos, Vector2D vel) : position(pos), velocity(vel), direction(0) {}

  virtual void update(float dt) = 0;
  virtual void render()         = 0;

public:
  Vector2D  position;
  Vector2D  velocity;
  Dimension dimension;
  int       direction;
};

class PlayerObject : public GameObject {
public:
  PlayerObject(const PlayerObject&) = delete;
  void update(float dt) override;
  void render() override;
};