#pragma once

#include "collider_component.hpp"
#include "texture_component.hpp"

class TileComponent {
public:
  TileComponent(SDL_Texture* texture, Vector2D pos, Vector2D dim)
      : texture_comp(texture, pos, dim), collider_comp(pos, dim) {}

  void update(float dt) { collider_comp.position = texture_comp.get_pos(); }

  void render(SDL_Renderer* renderer, const Camera& camera) {
    texture_comp.render(renderer, camera);
  }

  void set_position(Vector2D pos) {
    texture_comp.set_position(pos);
    collider_comp.set_position(pos);
  }

  ColliderComponent& get_collider_component() { return collider_comp; }
  TextureComponent&  get_texture_component() { return texture_comp; }

public:
  TextureComponent  texture_comp;
  ColliderComponent collider_comp;
};