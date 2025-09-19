#pragma once

#include "shared.hpp"

class Camera {
public:
  Camera() = default;
  Camera(Vector2 position, Vector2 dimension) : position(position), dimension(dimension) {};
  Camera(Vector2 position, Vector2 dimension, float left_margin, float right_margin)
      : position(position), dimension(dimension), left_margin(left_margin),
        right_margin(right_margin) {};

  void follow(const Vector2& target_pos) {
    if (target_pos.x - position.x < left_margin) {
      position.x = target_pos.x - left_margin;
    } else if (target_pos.x - position.x > right_margin) {
      position.x = target_pos.x - right_margin;
    }
  }

  void update(float min_horizontal, float max_horizontal) {
    if (position.x < min_horizontal) {
      position.x = min_horizontal;
    } else if (position.x + dimension.x > max_horizontal) {
      position.x = max_horizontal - dimension.x;
    }
  }

  void resize(float screen_width) {
    left_margin  = screen_width * LEFT_MARGIN_PCT;
    right_margin = screen_width * RIGHT_MARGIN_PCT;
  }

  const Vector2& get_position() const { return position; }

private:
  Vector2 position;
  Vector2 dimension;
  float   left_margin = 0, right_margin = 0;

public:
  static constexpr float LEFT_MARGIN_PCT  = 0.15;
  static constexpr float RIGHT_MARGIN_PCT = 0.65;
};
