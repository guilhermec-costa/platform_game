#include "../shared.hpp"

class ColliderComponent {

public:
  ColliderComponent() = default;
  ColliderComponent(Vector2D pos, Vector2D dim) : position(pos), dimension(dim) {};

public:
  Vector2D position;
  Vector2D dimension;

  bool check_collision(const Vector2D& other_pos, const Vector2D& other_size) {
    return !(other_pos.x + other_size.x < position.x || other_pos.x > position.x + dimension.x ||
             other_pos.y + other_size.y < position.y || other_pos.y > position.y + dimension.y);
  }
};