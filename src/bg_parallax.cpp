#include "../include/bg_parallax.hpp"

#include "../include/sdl_backend.hpp"
#include "../include/asset_manager/texture_manager.hpp"

#include <SDL2/SDL_render.h>

BackgroundParallax::BackgroundParallax() {
  bg_layer          = TextureManager::get_texture(TextureManager::asset_path(
      "assets/images/parallax/bg.png"));
  far_trees_layer   = TextureManager::get_texture(TextureManager::asset_path(
      "assets/images/parallax/far-trees.png"));
  mid_trees_layer   = TextureManager::get_texture(TextureManager::asset_path(
      "assets/images/parallax/mid-trees.png"));
  close_trees_layer = TextureManager::get_texture(TextureManager::asset_path(
      "assets/images/parallax/close-trees.png"));
  layers            = {{bg_layer, 0.1f, 0.0f},
                       {far_trees_layer, 0.3f, 0.0f},
                       {mid_trees_layer, 0.5f, 0.0f},
                       {close_trees_layer, 0.9f, 0.0f}};
}

void BackgroundParallax::update(float camera_x) {
  for (auto& layer : layers) {
    layer.offset = -camera_x * layer.speed;
  }
}

void BackgroundParallax::render(SDL_Window* window, SDL_Renderer* renderer) {
  Vector2 win_dim = SDLBackend::get_window_dimension(window);

  for (auto& layer : layers) {
    if (!layer.texture)
      continue;
    float    x     = fmod(layer.offset, (float)win_dim.x);
    SDL_Rect rect1 = {(int)x, 0, (int)win_dim.x, (int)win_dim.y};
    SDL_Rect rect2 = {(int)x + (int)win_dim.x, 0, (int)win_dim.x, (int)win_dim.y};
    SDL_RenderCopy(renderer, layer.texture, nullptr, &rect1);
    SDL_RenderCopy(renderer, layer.texture, nullptr, &rect2);
  }
}