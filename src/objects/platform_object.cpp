#include "../../include/objects/platform_object.hpp"

#include "../../include/asset_manager/texture_manager.hpp"
#include "../../include/asset_manager/utils.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

PlatformObject::PlatformObject(Vector2 pos, Vector2 dim) : GameObject(pos, {0, 0}, dim) {
  auto texture      = Managers::TextureManager::get_texture(asset_path("assets/images/grass.png"));
  texture_component = Components::TextureComponent(texture, pos, dim);

  float collision_offset = dim.y * 0.35f;
  collider_component     = Components::ColliderComponent({pos.x, pos.y + collision_offset},
                                                         {dim.x, dim.y - collision_offset});
}
void PlatformObject::handle_event() {}

void PlatformObject::render(SDL_Renderer* renderer, const Camera& camera) {
  SDL_Rect rec{static_cast<int>(position.x) - static_cast<int>(camera.get_position().x),
               static_cast<int>(position.y), static_cast<int>(dimension.x),
               static_cast<int>(dimension.y)};
  SDL_RenderCopy(renderer, texture_component.get_texture(), NULL, &rec);
  collider_component.render_collision_box(renderer, camera);
}

RectOverlap PlatformObject::get_overlap(const SDL_Rect& rect) {
  const auto& collider_rect = collider_component.get_rect();
  int overlap_left   = (rect.x + rect.w) - collider_rect.x;
  int overlap_right  = (collider_rect.x + collider_rect.w) - rect.x;
  int overlap_top    = (rect.y + rect.h) - collider_rect.y;
  int overlap_bottom = (collider_rect.y + collider_rect.h) - rect.y;
  return RectOverlap {overlap_left,overlap_right,overlap_top, overlap_bottom};
}

void PlatformObject::update(float dt) {
  float collision_offset = dimension.y * 0.35f;
  collider_component.set_position({position.x, position.y + collision_offset});
}