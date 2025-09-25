#include "../../include/components/collider_component.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "../../include/asset_manager/font_manager.hpp"
namespace Components {

  ColliderComponent::ColliderComponent() = default;

  ColliderComponent::ColliderComponent(Vector2            pos,
                                       Vector2            dim,
                                       Vector2            collision_offset,
                                       const std::string& text) :
    position(pos + collision_offset), dimension(dim), collision_offset(collision_offset) {
    collision_box = {(int)position.x, (int)position.y, (int)dim.x, (int)dim.y};
    label         = std::make_unique<UI::Label>(
        text,
        FontManager::instance().get_asset("assets/fonts/YoungSerif-Regular.ttf"),
        Vector2(pos.x, dim.y - 20),
        Vector2(80, 80));
  }

  ColliderComponent::~ColliderComponent() {}

  bool ColliderComponent::check_collision(const SDL_Rect& other) const {
    SDL_Rect self = get_rect();
    return SDL_HasIntersection(&self, &other);
  }

  SDL_Rect ColliderComponent::get_rect() const {
    return SDL_Rect{static_cast<int>(position.x * scale.x),
                    static_cast<int>(position.y * scale.y),
                    static_cast<int>(dimension.x * scale.x),
                    static_cast<int>(dimension.y * scale.y)};
  }

  void ColliderComponent::render_collision_box(SDL_Renderer*       renderer,
                                               const Core::Camera& camera,
                                               const bool          follow_camera) {
    collision_box = get_rect();
    if (follow_camera) {
      collision_box.x -= camera.get_position().x;
      collision_box.y -= camera.get_position().y;
    }

    SDL_SetRenderDrawColor(
        renderer, drawing_color.r, drawing_color.g, drawing_color.b, drawing_color.a);
    SDL_RenderDrawRect(renderer, &collision_box);

    Vector2 label_pos = {position.x * scale.x, position.y * scale.y - 20};
    if (follow_camera)
      label_pos -= camera.get_position();
    label->set_position(label_pos);
    label->render(renderer);
  }

  void ColliderComponent::set_position(const Vector2& pos) {
    position = pos + collision_offset;
  }

  void ColliderComponent::set_dimension(const Vector2& dim) {
    dimension = dim;
  }

  void ColliderComponent::flip_to_left() {
    collision_offset.x = -std::abs(collision_offset.x);
    position.x         = position.x + collision_offset.x;
  }

  void ColliderComponent::flip_to_right() {
    collision_offset.x = std::abs(collision_offset.x);
    position.x         = position.x + collision_offset.x;
  }

  void ColliderComponent::set_drawing_color(const SDL_Color color) {
    drawing_color = color;
  }

  void ColliderComponent::set_scale(const Vector2& s) {
    scale = s;
  }

} // namespace Components
