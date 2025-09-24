#pragma once

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "../../include/level.hpp"
#include "character_object.hpp"

enum class MovementState { IDLE, RUNNING, JUMPING, FALLING };
enum class ActionState { NONE, ATTACKING };

enum class PlayerEvent { JUMP, MOVE_LEFT, MOVE_RIGHT, STOP_HORIZONTAL, ATTACK };
enum class PlayerAnimation { RUN, JUMP, IDLE, ATTACK };
enum class PlayerState {
  IDLEING,
  RUNNING,
  JUMPING,
  ATTACKING,
};

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
  void check_player_ground_collision();
  void check_player_window_collision();

public:
  float jump_force;

private:
  std::unique_ptr<Components::ColliderComponent> sword_collider_component;
  PlayerState                                    state;
  MovementState                                  movement_state = MovementState::IDLE;
  ActionState                                    action_state   = ActionState::NONE;
  PlayerData                                     m_metadata;
};