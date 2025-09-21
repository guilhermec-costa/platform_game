#pragma once

#include <SDL2/SDL_render.h>

#include "components/collider_component.hpp"

class Ground {
public:
  Ground() = default;
  Ground(int screen_w, int screen_h, float y_pos, int tile_w);

  void update(float camera_x) {
    update_collider(camera_x);
  }
  void render();
  void resize(int new_width, int new_height);

  const Components::ColliderComponent& get_collider_component() const {
    return collider;
  }

private:
  void update_collider(float camera_x);

private:
  Components::ColliderComponent collider;
  SDL_Texture*                  ground_tile   = nullptr;
  float                         y             = 0;
  int                           screen_width  = 0;
  int                           screen_height = 0;
  int                           tile_side     = 64;
};
