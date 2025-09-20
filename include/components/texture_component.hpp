#pragma once

#include "../camera.hpp"
#include "../shared.hpp"

#include <SDL2/SDL.h>

namespace Components {

class TextureComponent {
public:
  TextureComponent();
  TextureComponent(SDL_Texture* texture, Vector2 pos, Vector2 dim);

  void render(SDL_Renderer* renderer, const Core::Camera& camera);
  void set_position(Vector2 pos);
  void set_dimension(Vector2 dim);
  void scale(float proportion);

  const Vector2& get_pos() const;
  const Vector2& get_dimension() const;
  SDL_Texture*   get_texture();

private:
  SDL_Texture* m_texture = nullptr;
  SDL_Rect     src_rect;
  SDL_Rect     target_rect;
  Vector2      pos;
  Vector2      dim;
};

} // namespace Components
