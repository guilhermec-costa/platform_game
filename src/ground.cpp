#include "../include/ground.hpp"

#include "../include/asset_manager/texture_manager.hpp"
#include "../include/game_context.hpp"

Ground::Ground(int screen_w, int screen_h, float y_pos, int tile_w = 64) :
  y(y_pos), screen_width(screen_w), screen_height(screen_h), tile_side(tile_w) {

  ground_tile = Managers::TextureManagerSingleton::instance().get_asset("assets/images/grass.png");
  update_collider(0);
}

void Ground::render() {
  auto& ctx = Core::GameContext::instance();
  if (!ground_tile)
    return;

  int num_tiles = screen_width / tile_side + 2;
  for (int i = 0; i < num_tiles; ++i) {
    int      x    = i * tile_side - (static_cast<int>(ctx.camera.get_position().x) % tile_side);
    SDL_Rect rect = {x, static_cast<int>(y), tile_side, tile_side};
    SDL_RenderCopy(ctx.renderer, ground_tile, nullptr, &rect);
  }
  // collider.render_collision_box(ctx.renderer, ctx.camera);
}

void Ground::resize(int new_width, int new_height) {
  screen_width  = new_width;
  screen_height = new_height;
  y             = new_height - tile_side;
  update_collider(0);
}

void Ground::update_collider(float camera_x) {
  int   num_tiles        = screen_width / tile_side + 2;
  float collision_offset = tile_side * 0.35f;

  collider.set_position({0.0f, static_cast<float>(screen_height - tile_side + collision_offset)});
  collider.set_dimension({static_cast<float>(num_tiles * tile_side) + camera_x,
                          static_cast<float>(tile_side - collision_offset)});
}
