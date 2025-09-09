#pragma once

#include "camera.hpp"
#include "components/collider_component.hpp"
#include "sdl_backend.hpp"
#include "texture_manager.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

class Ground {
public:
  Ground() = default;

  Ground(SDL_Renderer* renderer, SDL_Window* window, float y_pos, int tile_width = 64)
      : y(y_pos), screen_width(SDLBackend::get_window_width(window)),
        num_tiles(screen_width / tile_width + 2),
        screen_height(SDLBackend::get_window_height(window)), tile_width(tile_width) {
    ground_tile = TextureManager::get_instance().load_texture("assets/grass.png", renderer);
    collider =
        ColliderComponent({0, screen_height - 36 * 1.0f}, {screen_width * 1.0f, tile_width * 1.0f});
  }

  void update() {
    num_tiles = screen_width / tile_width + 2;
    collider.set_dimension({num_tiles * tile_width * 1.0f, collider.dimension.y});
  }
  void render(SDL_Renderer* renderer, const Camera& camera) {
    if (!ground_tile)
      return;

    for (int i = 0; i < num_tiles; ++i) {
      int      x    = i * tile_width - ((int)camera.x % tile_width);
      SDL_Rect rect = {x, (int)y, tile_width, tile_width};
      SDL_RenderCopy(renderer, ground_tile, nullptr, &rect);
    }
  }

  inline ColliderComponent& get_collider_component() { return collider; }

private:
  ColliderComponent collider;
  SDL_Texture*      ground_tile   = nullptr;
  float             y             = 0;
  int               screen_width  = 0;
  int               screen_height = 0;
  int               tile_width    = 64;
  int               num_tiles     = 0;
};
