#pragma once

#include <SDL2/SDL_events.h>

#include "../level.hpp"
#include "character_object.hpp"

class MonsterObject : public CharacterObject {
public:
  MonsterObject() = delete;
  MonsterObject(const LevelData::MonsterData& data = LevelData::MonsterData());

  void handle_event(const SDL_Event& event);
  void update(float dt) override;
  void render(SDL_Renderer* renderer, const Core::Camera& camera) override;
  void update_animation(float dt) override;
  void update_state() override;
  void resize();

private:
  float patrol_min_x, patrol_max_x = 0;
  int   direction = 1;
};