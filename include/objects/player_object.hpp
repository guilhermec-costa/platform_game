#pragma once

#include "../components/animated_sprite_component.hpp"
#include "../components/collider_component.hpp"
#include "../texture_manager.hpp"
#include "game_object.hpp"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

enum PlayerEvent { JUMP, MOVE_LEFT, MOVE_RIGHT, STOP_HORIZONTAL, LAND };

class PlayerObject : public GameObject {
public:
  PlayerObject() = delete;
  PlayerObject(Vector2D pos, Vector2D dim)
      : GameObject(pos, {0, 0}, dim), on_ground(true), move_speed(300.0f), jump_force(600.0f),
        base_height(pos.y), gravity(1200.0f) {

    int  tex_width, tex_height;
    auto texture = TextureManager::get_instance().get_texture("assets/nigthborne.png");
    SDL_QueryTexture(texture, nullptr, nullptr, &tex_width, &tex_height);

    int frame_width  = 80;
    int frame_height = 80;
    int num_columns  = tex_width / frame_width;
    int num_rows     = tex_height / frame_height;
    int total_frames = num_columns * num_rows;

    animated_sprite = AnimatedSpriteComponent(
        TextureComponent(texture, {0, 0}, {tex_width * 1.0f, tex_height * 1.0f}), frame_width,
        frame_height, 55, num_columns, 0.1, {dim.x, dim.y});
    collider_comp = ColliderComponent(pos, dim);
  }

  void        handle_event(PlayerEvent event);
  void        update(float dt) override;
  void        render(SDL_Renderer* renderer, const Camera& camera) override;
  inline void set_on_ground(bool state) { on_ground = state; }

public:
  ColliderComponent       collider_comp;
  AnimatedSpriteComponent animated_sprite;
  float                   base_height;
  bool                    on_ground;

private:
  float move_speed;
  float jump_force;
  float gravity;
};