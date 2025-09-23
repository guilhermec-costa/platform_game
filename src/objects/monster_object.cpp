#include "../../include/objects/monster_object.hpp"

#include "../../include/asset_manager/texture_manager.hpp"

MonsterObject::MonsterObject(const LevelData::MonsterData& data) :
  GameObject(data.position, {0, 0}, data.dimension), gravity(500.0f) {
  auto texture = Managers::TextureManagerSingleton::instance().get_asset(
      "assets/images/enemy1_spritesheet.png");
  Vector2 tex_dim = Managers::TextureManagerSingleton::instance().get_texture_dimension(texture);

  Vector2 collider_dim{dimension.x * 0.5f, dimension.y * 0.7f};
  collider_component = Components::ColliderComponent(
      data.position, collider_dim, {dimension.x * 0.25f, dimension.y * 0.1f});

  animated_sprite = Components::AnimatedSpriteComponent(
      Components::TextureComponent(texture, {0, 0}, {tex_dim.x, tex_dim.y}),
      140,
      93,
      0.1,
      data.dimension);

  animated_sprite.add_animation(
      static_cast<int>(MonsterAnimation::IDLE), "idle", 0, 7, 0.099f, true);
  animated_sprite.add_animation(
      static_cast<int>(MonsterAnimation::ATTACK), "attack", 9, 15, 0.099f, false);
}

void MonsterObject::update(float dt) {
  apply_gravity(dt);
  move(dt);
  update_collider();
  update_animation(dt);
  update_state();
}

void MonsterObject::apply_gravity(float dt) {
  velocity.y += gravity * dt;
  position.y += velocity.y * dt;

  const SDL_Rect& player_rect = collider_component.get_rect();
  const SDL_Rect& ground_rect =
      Core::GameContext::instance().global_ground.get_collider_component().get_rect();

  if (SDL_HasIntersection(&player_rect, &ground_rect)) {
    float ground_top = static_cast<float>(ground_rect.y);
    land_on(ground_top);
  } else {
    set_on_ground(false);
  }
}

void MonsterObject::move(float dt) {
  position.x += velocity.x * dt;
}

void MonsterObject::update_collider() {
  collider_component.set_position(position);
}

void MonsterObject::update_animation(float dt) {
  animated_sprite.set_flipped(velocity.x < 0);
  switch (movement_state) {
    case MovementState::IDLE:
      animated_sprite.play_animation((int)MonsterAnimation::IDLE);
      break;
    default:
      break;
  }
  animated_sprite.update(dt);
}

void MonsterObject::update_state() {
  movement_state = MovementState::JUMPING;
}

void MonsterObject::land_on(float surface_y) {
  position.y = surface_y - collider_component.dimension.y;
  velocity.y = 0;
}

void MonsterObject::resize() {
  // caso queira adaptar tamanho conforme escala da tela
}

void MonsterObject::render(SDL_Renderer* renderer, const Core::Camera& camera) {
  animated_sprite.render(renderer, position, camera);
  collider_component.render_collision_box(renderer, camera);
}

void MonsterObject::handle_event(const SDL_Event& event) {
  // atualmente não reage a eventos, mas pode reagir a triggers
}
