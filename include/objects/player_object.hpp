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
      : GameObject(pos, {0, 0}, dim), on_ground(true), move_speed(300.0f), jump_force(600.0f),
        base_height_location(pos.y), gravity(1200.0f) {

    auto texture = TextureManager::get_instance().get_texture(
        TextureManager::asset_path("assets/nigthborne.png"));
    Vector2D tex_dim = SDLBackend::get_texture_dimensions(texture);
    collider_comp    = ColliderComponent({pos.x + (dim.x * .45f), pos.y + (dim.y * .45f)},
                                         {dim.x * .55f, dim.y * .55f});

    animated_sprite = AnimatedSpriteComponent(
        TextureComponent(texture, {0, 0}, {tex_dim.x, tex_dim.y}), 80, 80, 0.1, {dim.x, dim.y});

    animated_sprite.add_animation(PlayerAnimation::IDLE, "idle", 0, 8, 0.1f);
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
  float                   base_height_location;
  bool                    on_ground;

private:
  float move_speed;
  float jump_force;
  float gravity;
};