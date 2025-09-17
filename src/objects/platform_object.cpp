#include "../../include/objects/platform_object.hpp"

#include <SDL2/SDL_render.h>

void PlatformObject::handle_event() {}

void PlatformObject::render(SDL_Renderer* renderer, const Camera& camera) {
  SDL_Rect rec{static_cast<int>(position.x) - static_cast<int>(camera.get_position().x), static_cast<int>(position.y),
               static_cast<int>(dimension.x), static_cast<int>(dimension.y)};
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderDrawRect(renderer, &rec);
}

void PlatformObject::update(float dt) {}