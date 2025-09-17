#pragma once

#include "asset_manager/texture_manager.hpp"
#include "asset_manager/utils.hpp"
#include "camera.hpp"
#include "components/collider_component.hpp"

#include <SDL2/SDL_render.h>

class Ground {
public:
  Ground() = default;

  Ground(SDL_Renderer* renderer, int screen_w, int screen_h, float y_pos, int tile_w = 64)
      : y(y_pos), screen_width(screen_w), screen_height(screen_h), tile_side(tile_w) {

    ground_tile = TextureManager::get_texture(asset_path("assets/images/grass.png"));
    update_collider(0);
  }

  void update(float camera_x) { update_collider(camera_x); }

  void render(SDL_Renderer* renderer, const Camera& camera) {
    if (!ground_tile)
      return;

    int num_tiles = screen_width / tile_side + 2;
    for (int i = 0; i < num_tiles; ++i) {
      int      x    = i * tile_side - (static_cast<int>(camera.get_position().x) % tile_side);
      SDL_Rect rect = {x, static_cast<int>(y), tile_side, tile_side};
      SDL_RenderCopy(renderer, ground_tile, nullptr, &rect);
    }
  }

  void resize(int new_width, int new_height) {
    screen_width  = new_width;
    screen_height = new_height;
    y             = new_height - tile_side;
    update_collider(0);
  }

  const ColliderComponent& get_collider_component() const { return collider; }

private:
  void update_collider(float camera_x) {
    int   num_tiles        = screen_width / tile_side + 2;
    float collision_offset = tile_side * 0.35f;

    collider.set_position({0.0f, static_cast<float>(screen_height - tile_side + collision_offset)});
    collider.set_dimension({static_cast<float>(num_tiles * tile_side) + camera_x,
                            static_cast<float>(tile_side - collision_offset)});
  }

private:
  ColliderComponent collider;
  SDL_Texture*      ground_tile   = nullptr;
  float             y             = 0;
  int               screen_width  = 0;
  int               screen_height = 0;
  int               tile_side     = 64;
};
