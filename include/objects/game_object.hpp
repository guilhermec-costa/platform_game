#pragma once

#include <SDL2/SDL_render.h>

#include "../camera.hpp"
#include "../game_context.hpp"
#include "../shared.hpp"

class GameObject {
public:
  GameObject() : position(0, 0), velocity(0, 0) {}
  GameObject(const GameObject&) = delete;
  GameObject(Vector2 pos, Vector2 vel, Vector2 dimension) :
    position(pos), velocity(vel), dimension(dimension) {}
  virtual void update(float dt)                                           = 0;
  virtual void render(SDL_Renderer* renderer, const Core::Camera& camera) = 0;

  const Components::ColliderComponent& get_collider_component() {
    return collider_component;
  }

protected:
  Core::GameContext& ctx = Core::GameContext::instance();
  Managers::AudioManager&             audio_manager  = Core::GameContext::instance().audio_manager;
  Components::ColliderComponent       collider_component;

public:
  Vector2 position;
  Vector2 velocity;
  Vector2 dimension;
};
