#pragma once

#include "../components/animated_sprite_component.hpp"
#include "../components/collider_component.hpp"
#include "../sdl_backend.hpp"
#include "../texture_manager.hpp"
#include "game_object.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

enum PlayerEvent { JUMP, MOVE_LEFT, MOVE_RIGHT, STOP_HORIZONTAL, LAND };
enum PlayerAnimation { RUNNING, JUMPING, IDLE };

class PlayerObject : public GameObject {
public:
  PlayerObject() = delete;
  PlayerObject(Vector2D pos, Vector2D dim)
      : GameObject(pos, {0, 0}, dim), on_ground(true),
        base_height_location(pos.y) {

    auto texture = TextureManager::get_instance().get_texture(
        TextureManager::asset_path("assets/nigthborne.png"));
    Vector2D tex_dim = SDLBackend::get_texture_dimensions(texture);

    collider_offset = {dimension.x * 0.25f, dimension.y * 0.25f};
    Vector2D collider_dim{dimension.x * 0.55f, dimension.y * 0.55f};
    collider_comp = ColliderComponent(pos + collider_offset, collider_dim);

    animated_sprite = AnimatedSpriteComponent(
        TextureComponent(texture, {0, 0}, {tex_dim.x, tex_dim.y}), 80, 80, 0.1, dim);

    animated_sprite.add_animation(PlayerAnimation::IDLE, "idle", 0, 8, 0.099f);
    animated_sprite.add_animation(PlayerAnimation::RUNNING, "run", 8, 12, 0.1f);
    animated_sprite.add_animation(PlayerAnimation::JUMPING, "jump", 0, 8, 0.1f);
  }

  void        handle_event(PlayerEvent event);
  void        update(float dt) override;
  void        render(SDL_Renderer* renderer, const Camera& camera) override;
  inline void set_on_ground(bool state) { on_ground = state; }

public:
  ColliderComponent       collider_comp;
  AnimatedSpriteComponent animated_sprite;
  Vector2D collider_offset;
  float                   base_height_location;
  bool                    on_ground;

public:
  static constexpr float MOVE_SPEED      = 300.0f;
  static constexpr float JUMP_FORCE      = 600.0f;
  static constexpr float GRAVITY         = 1200.0f;
  static constexpr float MAX_FALL_SPEED  = 800.0f;
};