#include "../../include/components/texture_component.hpp"

namespace Components {

  TextureComponent::TextureComponent() = default;

  TextureComponent::TextureComponent(SDL_Texture* texture, Vector2 pos, Vector2 dim) :
    m_texture(texture), pos(pos), dim(dim) {
    src_rect    = {0, 0, (int)dim.x, (int)dim.y};
    target_rect = {(int)pos.x, (int)pos.y, (int)dim.x, (int)dim.y};
  }

  void TextureComponent::render(SDL_Renderer* renderer, const Core::Camera& camera) {
    if (!m_texture)
      return;

    SDL_RenderCopy(renderer, m_texture, &src_rect, &target_rect);
  }

  void TextureComponent::set_position(Vector2 new_pos) {
    pos           = new_pos;
    target_rect.x = (int)pos.x;
    target_rect.y = (int)pos.y;
  }

  void TextureComponent::set_dimension(Vector2 new_dim) {
    dim           = new_dim;
    target_rect.w = (int)dim.x;
    target_rect.h = (int)dim.y;
  }

  void TextureComponent::scale(float proportion) {
    target_rect.w = (int)(target_rect.w * proportion);
    target_rect.h = (int)(target_rect.h * proportion);
  }

  const Vector2& TextureComponent::get_pos() const {
    return pos;
  }

  const Vector2& TextureComponent::get_dimension() const {
    return dim;
  }

  SDL_Texture* TextureComponent::get_texture() {
    return m_texture;
  }

} // namespace Components
