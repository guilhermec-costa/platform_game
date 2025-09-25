#pragma once

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "../../include/level.hpp"
#include "character_object.hpp"

enum class PlayerEvent { JUMP, MOVE_LEFT, MOVE_RIGHT, STOP_HORIZONTAL, ATTACK };

class PlayerObject : public CharacterObject {
public:
  PlayerObject() = delete;
  PlayerObject(const PlayerData& player_data);

  void handle_event(PlayerEvent event);
  void update(float dt) override;
  void render(SDL_Renderer* renderer, const Core::Camera& camera) override;
  void resize();
  void update_state() override;
  void update_animation(float dt) override;
  void check_player_window_collision();

public:
  float jump_force;

private:
  std::unique_ptr<Components::ColliderComponent> sword_collider_component;
  PlayerData                                     m_metadata;
};