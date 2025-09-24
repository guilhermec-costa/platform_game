#include "../../include/objects/monster_object.hpp"

#include "../../include/asset_manager/texture_manager.hpp"
#include "../../include/components/component_factory.hpp"
#include "../../include/logger.hpp"

MonsterObject::MonsterObject(const LevelData::MonsterData& data) :
  CharacterObject(data.position, data.dimension) {
  auto texture = Managers::TextureManagerSingleton::instance().get_asset(
      "assets/images/enemy1_spritesheet.png");
  Vector2 tex_dim = Managers::TextureManagerSingleton::instance().get_texture_dimension(texture);

  Vector2 collider_dim{dimension.x * 0.5f, dimension.y * 0.7f};
  collider_component = ComponentFactory::make_collider(
      data.position, collider_dim, Vector2{dimension.x * 0.25f, dimension.y * 0.1f});

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

  gravity    = data.gravity;
  move_speed = data.move_speed;
  LOG_TRACE(to_string());
}

void MonsterObject::check_player_ground_collision() {
  const SDL_Rect& monster_collider_rect = collider_component->get_rect();
  const SDL_Rect& ground_rect =
      Core::GameContext::instance().global_ground->get_collider_component().get_rect();

  if (SDL_HasIntersection(&monster_collider_rect, &ground_rect)) {
    float ground_top = static_cast<float>(ground_rect.y);
    land_on(ground_top);
  } else {
    set_on_ground(false);
  }
}

void MonsterObject::update(float dt) {
  CharacterObject::update(dt);
  check_player_ground_collision();
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

void MonsterObject::update_state() {}

void MonsterObject::resize() {}

void MonsterObject::render(SDL_Renderer* renderer, const Core::Camera& camera) {
  animated_sprite.render(renderer, position, camera);
  collider_component->render_collision_box(renderer, camera);
}

void MonsterObject::handle_event(const SDL_Event& event) {}
