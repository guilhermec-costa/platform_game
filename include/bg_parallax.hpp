#pragma once

#include <SDL2/SDL_render.h>
#include <vector>

struct Layer {
  SDL_Texture* texture;
  float        speed;
  float        offset;
};

class BackgroundParallax {
public:
  BackgroundParallax();
  BackgroundParallax(const BackgroundParallax&) = delete;
  void update(float camera_x);
  void render(SDL_Renderer* renderer);

private:
  SDL_Texture*       bg_layer          = nullptr;
  SDL_Texture*       far_trees_layer   = nullptr;
  SDL_Texture*       mid_trees_layer   = nullptr;
  SDL_Texture*       close_trees_layer = nullptr;
  float              bg_offset = 0.0f, far_offset = 0.0f, mid_offset = 0.0f, close_offset = 0.0f;
  std::vector<Layer> layers;
};