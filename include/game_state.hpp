#pragma once

#include "game_context.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

class GameState {
protected:
  GameContext& context;

public:
  GameState(GameContext& ctx) : context(ctx) {}
  virtual void handle_event(SDL_Event& event) = 0;
  virtual void update(float dt)               = 0;
  virtual void render()                       = 0;
  virtual ~GameState() {}
};

class PlayState : public GameState {
public:
  void handle_event(SDL_Event& event) override;
  void update(float dt) override;
  void render() override;
};