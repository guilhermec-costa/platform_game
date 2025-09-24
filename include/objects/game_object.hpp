#pragma once

#include <sstream>

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

  const Components::ColliderComponent& get_collider_component() const {
    return *collider_component.get();
  }

  virtual std::string to_string() const {
    std::ostringstream oss;
    oss << "GameObject { "
        << "position=(" << position.x << ", " << position.y << "), "
        << "velocity=(" << velocity.x << ", " << velocity.y << "), "
        << "dimension=(" << dimension.x << ", " << dimension.y << ")";

    const SDL_Rect rect = collider_component->get_rect();
    oss << ", collider=(" << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << ") }";

    return oss.str();
  }

protected:
  Core::GameContext&      ctx           = Core::GameContext::instance();
  Managers::AudioManager& audio_manager = Core::GameContext::instance().audio_manager;
  std::unique_ptr<Components::ColliderComponent> collider_component;

public:
  Vector2 position;
  Vector2 velocity;
  Vector2 dimension;
};
