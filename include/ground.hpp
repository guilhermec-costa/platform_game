#include "camera.hpp"
#include "components/tile_component.hpp"
#include "texture_manager.hpp"

#include <SDL2/SDL_render.h>

class Ground {
public:
  Ground() = default;
  Ground(SDL_Renderer* renderer, float y_pos, int screen_width, int tile_width = 64)
      : y(y_pos), width(screen_width), tile_width(tile_width) {
    ground_tile = TextureManager::get_instance().load_texture("assets/grass.png", renderer);
  }

  void render(SDL_Renderer* renderer, const Camera& camera) {
    if (!ground_tile)
      return;

    int num_tiles = (width / tile_width) + 5;

    for (int i = 0; i < num_tiles; ++i) {
      int      x    = i * tile_width - ((int)camera.x % tile_width);
      TileComponent tile(ground_tile, {x * 1.0f, y}, {tile_width * 1.0f, tile_width * 1.0f});
      tile.render(renderer, camera);
    }
  }

private:
  SDL_Texture* ground_tile = nullptr;
  float        y           = 0;
  int          width       = 0;
  int          tile_width  = 64;
};
