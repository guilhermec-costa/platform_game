#include "../../include/objects/platform_object.hpp"
#include <SDL2/SDL_render.h>

void PlatformObject::handle_event() {}

void PlatformObject::render(SDL_Renderer* renderer, const Camera& camera) {
  SDL_Rect rec{100, 100, 100, 100};
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderDrawRect(renderer, &rec);
}

void PlatformObject::update(float dt) {}