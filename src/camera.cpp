#include "../include/camera.hpp"

namespace Core {

  Camera::Camera(Vector2 position, Vector2 dimension, float left_margin, float right_margin) :
    position(position),
    dimension(dimension),
    left_margin(left_margin),
    right_margin(right_margin) {}

  void Camera::follow(const Vector2& target_pos) {
    if (target_pos.x - position.x < left_margin) {
      position.x = target_pos.x - left_margin;
    } else if (target_pos.x - position.x > right_margin) {
      position.x = target_pos.x - right_margin;
    }
  }

  void Camera::update(float min_horizontal, float max_horizontal) {
    if (position.x < min_horizontal) {
      position.x = min_horizontal;
    } else if (position.x + dimension.x > max_horizontal) {
      position.x = max_horizontal - dimension.x;
    }
  }

  void Camera::resize(float screen_width) {
    left_margin  = screen_width * LEFT_MARGIN_PCT;
    right_margin = screen_width * RIGHT_MARGIN_PCT;
    // dimension.y = screen_height;
  }

  void Camera::render(SDL_Renderer* renderer) {
    SDL_Rect cam_rect = {(int)position.x, (int)position.y, (int)dimension.x, (int)dimension.y};
    SDL_SetRenderDrawColor(renderer, 255, 120, 120, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(renderer, &cam_rect);
  }

  const Vector2& Camera::get_position() const {
    return position;
  }
} // namespace Core
