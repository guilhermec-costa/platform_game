#pragma once

#include <iostream>
#include <ostream>

typedef struct Vector2D {
  float x, y;

  Vector2D() : x(0), y(0) {};
  Vector2D(float x, float y) : x(x), y(y) {};
  Vector2D(const Vector2D& v) { std::cout << "Copying vector\n"; };

  Vector2D operator*(float scalar) const { return Vector2D(x * scalar, y * scalar); }

  friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
    os << "X: " << v.x << " | Y: " << v.y << "\n";
    return os;
  };

} Vector2D;

typedef struct Dimension {
private:
  Vector2D dimension;

public:
  Dimension() = default;
  Dimension(float w, float h) : dimension(Vector2D{w, h}) {};
  float get_w() { return dimension.x; };

  float get_h() { return dimension.y; };
} Dimension;