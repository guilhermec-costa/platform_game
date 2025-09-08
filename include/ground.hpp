#include "camera.hpp"
#include "components/tile_component.hpp"
#include "texture_manager.hpp"

#include <SDL2/SDL_render.h>
#include <vector>

class Ground {
public:
  Ground() = default;
  Ground(SDL_Renderer* renderer, float y_pos, int screen_width, int tile_width = 64)
      : y(y_pos), width(screen_width), tile_width(tile_width) {
    ground_tile_texture = TextureManager::get_instance().load_texture("assets/grass.png", renderer);
    int num_tiles       = (width / tile_width) + 2;
    tiles.reserve(num_tiles);

    for (int i = 0; i < num_tiles; ++i) {
      Vector2D pos(i * tile_width, y);
      Vector2D dim(tile_width, tile_width);
      tiles.emplace_back(ground_tile_texture, pos, dim);
    }
  }

  void render(SDL_Renderer* renderer, const Camera& camera) {
    if (!ground_tile_texture)
      return;

    for (auto& tile : tiles) {
      tile.render(renderer, camera);
    }
  }

  std::vector<TileComponent> get_tiles() { return tiles; }

private:
  SDL_Texture*               ground_tile_texture = nullptr;
  float                      y                   = 0;
  int                        width               = 0;
  int                        tile_width          = 64;
  std::vector<TileComponent> tiles;
};
