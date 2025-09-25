#include "../../include/objects/monster_object.hpp"

#include "../../include/asset_manager/texture_manager.hpp"
#include "../../include/components/component_factory.hpp"
#include "../../include/logger.hpp"

MonsterObject::MonsterObject(const LevelData::MonsterData& data) :
  CharacterObject(data.position, data.dimension),
  patrol_min_x(data.patrol_min_x),
  patrol_max_x(data.patrol_max_x) {
  
  auto texture = Managers::TextureManagerSingleton::instance().get_asset(
      "assets/images/enemy1_spritesheet.png");
  Vector2 tex_dim = Managers::TextureManagerSingleton::instance().get_texture_dimension(texture);

  collider_component = ComponentFactory::make_collider(
      data.collider.position,
      data.collider.dimension,
      Vector2{dimension.x * 0.25f, dimension.y * 0.1f},
      "Monster");

  animated_sprite = Components::AnimatedSpriteComponent(
      Components::TextureComponent(texture, {0, 0}, {tex_dim.x, tex_dim.y}),
      140,
      93,
      0.1,
      data.dimension);

  animated_sprite.add_animation("idle",   0,  7, 0.099f, true);
  animated_sprite.add_animation("walk",   8, 15, 0.12f);
  animated_sprite.add_animation("attack", 16, 24, 0.12f);

  gravity    = data.gravity;
  move_speed = data.move_speed;

  LOG_TRACE(to_string());
}


void MonsterObject::update(float dt) {
  velocity.x = move_speed * direction;
  position.x += velocity.x * dt;

  if (position.x < patrol_min_x) {
    position.x = patrol_min_x;
    direction  = 1;
  } else if (position.x > patrol_max_x) {
    position.x = patrol_max_x;
    direction  = -1;
  }

  velocity.y += gravity * dt;
  position.y += velocity.y * dt;
  collider_component->set_position(position);

  update_animation(dt);
  update_state();
}

void MonsterObject::update_animation(float dt) {
  if (action_state == ActionState::ATTACKING) {
    animated_sprite.play_animation("attack");
  } else {
    switch (movement_state) {
      case MovementState::WALKING: {
        animated_sprite.play_animation("walk");
        break;
      }
      case MovementState::IDLE: {
        animated_sprite.play_animation("idle");
        break;
      }
      default: {
        break;
      }
    }
  }
  animated_sprite.set_flipped(velocity.x > 0);
  animated_sprite.update(dt);
}

void MonsterObject::update_state() {
  if (velocity.x != 0) {
    movement_state = MovementState::WALKING;
  } else {
    movement_state = MovementState::IDLE;
  }
}

void MonsterObject::resize() {}

void MonsterObject::render(SDL_Renderer* renderer, const Core::Camera& camera) {
  animated_sprite.render(renderer, position, camera);
  collider_component->render_collision_box(renderer, camera);
}

void MonsterObject::handle_event(const SDL_Event& event) {}
