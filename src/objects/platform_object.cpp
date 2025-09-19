#include "../../include/objects/platform_object.hpp"

#include "../../include/asset_manager/texture_manager.hpp"
#include "../../include/asset_manager/utils.hpp"

#include <SDL2/SDL_render.h>

PlatformObject::PlatformObject(Vector2 pos, Vector2 dim) : GameObject(pos, {0, 0}, dim) {
  auto texture       = TextureManager::get_texture(asset_path("assets/images/grass.png"));
  texture_component  = Components::TextureComponent(texture, pos, dim);
  collider_component = Components::ColliderComponent(pos, dim);
}
void PlatformObject::handle_event() {}

void PlatformObject::render(SDL_Renderer* renderer, const Camera& camera) {
  SDL_Rect rec{static_cast<int>(position.x) - static_cast<int>(camera.get_position().x),
               static_cast<int>(position.y), static_cast<int>(dimension.x),
               static_cast<int>(dimension.y)};
  texture_component.render(renderer, camera);
}

void PlatformObject::update(float dt) {
  collider_component.set_position(position);
}