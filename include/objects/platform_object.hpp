#pragma once

#include "../components/collider_component.hpp"
#include "../components/texture_component.hpp"
#include "game_object.hpp"

class PlatformObject : public GameObject {
public:
  PlatformObject(Vector2 pos, Vector2 dim);
  void        handle_event();
  void        update(float dt) override;
  void        render(SDL_Renderer* renderer, const Core::Camera& camera) override;
  RectOverlap get_overlap(const SDL_Rect& rect);
  Components::ColliderComponent& get_collider() { return collider_component; }

private:
  Components::ColliderComponent collider_component;
  Components::TextureComponent  texture_component;
};