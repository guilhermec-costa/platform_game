#pragma once

#include "shared.hpp"

// struct Camera {
//   float x, y;
//   float w, h;
// };

class Camera {
public:
  Camera() = default;
  Camera(Vector2D position, Vector2D dimension): position(position), dimension(dimension){};

  void follow(const Vector2D& target_pos, float left_margin, float right_margin) {
    if (target_pos.x - position.x < left_margin) {
      position.x = target_pos.x - left_margin;
    } else if (target_pos.x - position.x > right_margin) {
      position.x = target_pos.x - right_margin;
    }
  }

  void update() {
    if (position.x < 0) {
      position.x = 0;
    }
  }

  const Vector2D& get_position() const {
    return position;
  }

private:
  Vector2D position;
  Vector2D dimension;
};
