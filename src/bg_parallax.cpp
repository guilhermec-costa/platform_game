#include "../include/bg_parallax.hpp"

#include <SDL2/SDL_render.h>

#include "../include/asset_manager/texture_manager.hpp"
#include "../include/game_context.hpp"

using namespace Managers;

BackgroundParallax::BackgroundParallax() {
  bg_layer = TextureManagerSingleton::instance().get_asset("assets/images/parallax/bg.png");
  far_trees_layer =
      TextureManagerSingleton::instance().get_asset("assets/images/parallax/far-trees.png");
  mid_trees_layer =
      TextureManagerSingleton::instance().get_asset("assets/images/parallax/mid-trees.png");
  close_trees_layer =
      TextureManagerSingleton::instance().get_asset("assets/images/parallax/close-trees.png");
  layers = {{.texture = bg_layer, .speed = 0.1f, .offset = 0.0f},
            {.texture = far_trees_layer, .speed = 0.3f, .offset = 0.0f},
            {.texture = mid_trees_layer, .speed = 0.5f, .offset = 0.0f},
            {.texture = close_trees_layer, .speed = 0.9f, .offset = 0.0f}};
}

void BackgroundParallax::update(float camera_x) {
  for (auto& layer : layers) {
    layer.offset = -camera_x * layer.speed;
  }
}

void BackgroundParallax::render() {
  auto&   ctx     = Core::GameContext::instance();
  Vector2 win_dim = ctx.window.get_dimension();

  for (const auto& layer : layers) {
    if (!layer.texture)
      continue;
    float    x     = fmod(layer.offset, (float)win_dim.x);
    SDL_Rect rect1 = {(int)x, 0, (int)win_dim.x, (int)win_dim.y};
    SDL_Rect rect2 = {(int)x + (int)win_dim.x, 0, (int)win_dim.x, (int)win_dim.y};
    SDL_RenderCopy(ctx.renderer, layer.texture, nullptr, &rect1);
    SDL_RenderCopy(ctx.renderer, layer.texture, nullptr, &rect2);
  }
}