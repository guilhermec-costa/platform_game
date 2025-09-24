#pragma once

#include <iostream>
#include <ostream>

typedef struct Vector2 {
  float x, y;

  Vector2() : x(0), y(0) {};
  Vector2(float _x, float _y) : x(_x), y(_y) {};
  Vector2(const Vector2& v) : x(v.x), y(v.y) {};

  Vector2 operator*(float scalar) const {
    return Vector2(x * scalar, y * scalar);
  }
  Vector2 operator+(float sum) const {
    return Vector2(x + sum, y + sum);
  };

  Vector2 operator-(const Vector2& minus) const {
    return Vector2(x - minus.x, y - minus.y);
  }
  Vector2 operator+(const Vector2& vec) const {
    return Vector2(x + vec.x, y + vec.y);
  };
  Vector2 operator+=(const Vector2& vec) {
    x += vec.x;
    y += vec.y;
    return *this;
  };

  Vector2 operator-=(const Vector2& vec) {
    x -= vec.x;
    y -= vec.y;
    return *this;
  };

  friend std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    os << "X: " << v.x << " | Y: " << v.y << "\n";
    return os;
  };

} Vector2;