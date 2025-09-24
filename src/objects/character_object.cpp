#include "../../include/objects/character_object.hpp"

CharacterObject::CharacterObject(const Vector2& pos, const Vector2& dim) :
  GameObject(pos, {0, 0}, dim), on_ground(false) {}

CharacterObject::~CharacterObject() = default;

void CharacterObject::update(float dt) {
  apply_gravity(dt);
  move(dt);
  update_collider();
  update_state();
  update_animation(dt);
}

void CharacterObject::update_collider() {
  collider_component->set_position(position);
}

void CharacterObject::land_on(float surface_y) {
  float new_y = surface_y - dimension.y + (dimension.y * land_offset_pct);
  position.y  = new_y;
  velocity.y  = 0;
  set_on_ground(true);
}

void CharacterObject::apply_gravity(float dt) {
  if (!on_ground) {
    velocity.y += gravity * dt;
    if (velocity.y > max_fall_speed) {
      velocity.y = max_fall_speed;
    }
  }
}

void CharacterObject::move(float dt) {
  position += velocity * dt;
}

std::string CharacterObject::to_string() const {
  std::ostringstream oss;
  oss << "CharacterObject { " << GameObject::to_string() << ", gravity=" << gravity
      << ", move_speed=" << move_speed << ", on_ground=" << (on_ground ? "true" : "false")
      << ", land_offset_pct=" << land_offset_pct << ", max_fall_speed=" << max_fall_speed << " }";
  return oss.str();
}