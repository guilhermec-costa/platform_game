#pragma once

#include "../camera.hpp"
#include "../shared.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

class ColliderComponent {

public:
  ColliderComponent() = default;
  ColliderComponent(Vector2D pos, Vector2D dim) : position(pos), dimension(dim) {
    collision_box = {(int)position.x, (int)position.y, (int)dim.x, (int)dim.y};
  };

public:
  Vector2D position;
  Vector2D dimension;
  SDL_Rect collision_box;

public:
  bool check_collision(const SDL_Rect& other) const {
    SDL_Rect self = get_rect();
    return SDL_HasIntersection(&self, &other);
  }

  SDL_Rect get_rect() const {
    return SDL_Rect{(int)position.x, (int)position.y, (int)dimension.x, (int)dimension.y};
  }

  void render_collision_box(SDL_Renderer* renderer, const Camera& camera,
                            const bool follow_camera = true) {
    collision_box = get_rect();
    if (follow_camera) {
      collision_box.x -= camera.get_position().x;
      collision_box.y -= camera.get_position().y;
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &collision_box);
  }

  void set_position(const Vector2D& pos) { position = pos; }
  void set_dimension(const Vector2D& dim) { dimension = dim; }
};