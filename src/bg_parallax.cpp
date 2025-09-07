#include "../include/bg_parallax.hpp"

#include "../include/sdl_backend.hpp"
#include "../include/texture_manager.hpp"

#include <SDL2/SDL_render.h>

BackgroundParallax::BackgroundParallax(SDL_Renderer* renderer)
    : bg_texture(nullptr), mid_trees_texture(nullptr), trees_texture(nullptr), bg_x(0.0f),
      mid_x(0.0f), trees_x(0.0f) {

  bg_texture = TextureManager::get_instance().load_texture(
      "assets/parallax_demon_woods_pack/layers/parallax-demon-woods-bg.png", renderer);
  far_trees_texture = TextureManager::get_instance().load_texture(
      "assets/parallax_demon_woods_pack/layers/parallax-demon-woods-far-trees.png", renderer);
  mid_trees_texture = TextureManager::get_instance().load_texture(
      "assets/parallax_demon_woods_pack/layers/parallax-demon-woods-mid-trees.png", renderer);
  trees_texture = TextureManager::get_instance().load_texture(
      "assets/parallax_demon_woods_pack/layers/parallax-demon-woods-close-trees.png", renderer);
}

void BackgroundParallax::update(float camera_x) {
  bg_x    = -camera_x * 0.2f;
  mid_x   = -camera_x * 0.5f;
  trees_x = -camera_x * 1.0f;
}

void BackgroundParallax::render(SDL_Window* window, SDL_Renderer* renderer) {
  Vector2D win_dim = SDLBackend::get_window_dimension(window);

  auto draw_layer = [&](SDL_Texture* tex, float offset) {
    if (!tex)
      return;

    float    x     = fmod(offset, (float)win_dim.x);
    SDL_Rect rect1 = {(int)x, 0, (int)win_dim.x, (int)win_dim.y};
    SDL_Rect rect2 = {(int)x + (int)win_dim.x, 0, (int)win_dim.x, (int)win_dim.y};

    SDL_RenderCopy(renderer, tex, nullptr, &rect1);
    SDL_RenderCopy(renderer, tex, nullptr, &rect2);
  };

  draw_layer(bg_texture, bg_x);
  draw_layer(far_trees_texture, mid_x);
  draw_layer(mid_trees_texture, mid_x);
  draw_layer(trees_texture, trees_x);
}
