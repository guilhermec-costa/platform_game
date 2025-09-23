#include "../../include/objects/monster_object.hpp"

#include "../../include/asset_manager/texture_manager.hpp"

MonsterObject::MonsterObject(const LevelData::MonsterData& data) : gravity(500.0f) {
  auto texture = Managers::TextureManagerSingleton::instance().get_asset(
      "assets/images/enemy1_spritesheet.png");
  Vector2 tex_dim = Managers::TextureManagerSingleton::instance().get_texture_dimension(texture);

  collider_component = Components::ColliderComponent(data.position, data.dimension);

  animated_sprite = Components::AnimatedSpriteComponent(
      Components::TextureComponent(texture, {0, 0}, {tex_dim.x, tex_dim.y}),
      80,
      80,
      0.1,
      data.dimension);

  animated_sprite.add_animation(static_cast<int>(1), "idle", 0, 8, 0.099f);
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

  float ground_y = 536;
  if (position.y + collider_component.dimension.y >= ground_y) {
    land_on(ground_y);
  }
}

void MonsterObject::move(float dt) {
  position.x += velocity.x * dt;

  // Patrulha: troca de direção ao bater em colisão lateral
  // if (collider_component.left_collision || collider_component.right_collision) {
  //     velocity.x = -velocity.x;
  // }
}

void MonsterObject::update_collider() {
  collider_component.position = position;
}

void MonsterObject::update_animation(float dt) {
  animated_sprite.update(dt);
  animated_sprite.set_flipped(velocity.x < 0);
}

void MonsterObject::update_state() {
  // futuramente pode adicionar: atacando, morto, perseguindo player
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
