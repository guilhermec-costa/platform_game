#pragma once

#include "../components/animated_sprite_component.hpp"
#include "../components/collider_component.hpp"
#include "game_object.hpp"

class PlatformObject : public GameObject {
public:
  PlatformObject(Vector2D pos, Vector2D dim) : GameObject(pos, {0, 0}, dim) {}

  void handle_event();
  void update(float dt) override;
  void render(SDL_Renderer* renderer, const Camera& camera) override;

private:
  ColliderComponent       collider_comp;
  AnimatedSpriteComponent animated_sprite;
};