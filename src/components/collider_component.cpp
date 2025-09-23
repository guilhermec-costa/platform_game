#include "../../include/components/collider_component.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

namespace Components {

  ColliderComponent::ColliderComponent() = default;

  ColliderComponent::ColliderComponent(Vector2 pos, Vector2 dim, Vector2 collision_offset) :
  position(pos + collision_offset), dimension(dim), collision_offset(collision_offset) {
  collision_box = {(int)position.x, (int)position.y, (int)dim.x, (int)dim.y};
}

  bool ColliderComponent::check_collision(const SDL_Rect& other) const {
    SDL_Rect self = get_rect();
    return SDL_HasIntersection(&self, &other);
  }

  SDL_Rect ColliderComponent::get_rect() const {
    return SDL_Rect{(int)position.x, (int)position.y, (int)dimension.x, (int)dimension.y};
  }

  void ColliderComponent::render_collision_box(SDL_Renderer*       renderer,
                                               const Core::Camera& camera,
                                               const bool          follow_camera) {
    collision_box = get_rect();
    if (follow_camera) {
      collision_box.x -= camera.get_position().x;
      collision_box.y -= camera.get_position().y;
    }
    SDL_SetRenderDrawColor(renderer, 125, 125, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(renderer, &collision_box);
  }

  void ColliderComponent::set_position(const Vector2& pos) {
    position = pos + collision_offset;
  }

  void ColliderComponent::set_dimension(const Vector2& dim) {
    dimension = dim;
  }

} // namespace Components
