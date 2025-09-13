#pragma once

#include <iostream>
#include <ostream>

typedef struct Vector2D {
  float x, y;

  Vector2D() : x(0), y(0) {};
  Vector2D(float _x, float _y) : x(_x), y(_y) {};
  Vector2D(const Vector2D& v) : x(v.x), y(v.y) {};

  Vector2D operator*(float scalar) const { return Vector2D(x * scalar, y * scalar); }
  Vector2D operator+(float sum) const { return Vector2D(x + sum, y + sum); };
  Vector2D operator+(Vector2D vec) const { return Vector2D(x + vec.x, y + vec.y); };

  friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
    os << "X: " << v.x << " | Y: " << v.y << "\n";
    return os;
  };

} Vector2D;